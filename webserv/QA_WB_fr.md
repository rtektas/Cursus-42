
## 1. Les bases d'un serveur HTTP

Un serveur HTTP se lie à un port TCP, écoute les connexions entrantes, lit les octets bruts du socket et les parse en une requête HTTP (méthode, chemin, en-têtes, corps), traite cette requête (servir un fichier statique, passer par un CGI, renvoyer une erreur), puis écrit une réponse HTTP (ligne de statut, en-têtes, corps) vers le client. Il est sans état (stateless) — chaque cycle requête/réponse est autonome.

---

## 2. Mécanisme d'événements

Nous utilisons **`poll()`**.

---

## 3. Fonctionnement de poll()

À chaque itération, nous construisons un nouveau vecteur `pollfd`. Chaque entrée contient un `fd`, les événements qui nous intéressent (`POLLIN`, `POLLOUT`), et un champ `revents` que le noyau remplit au retour. On appelle :

```cpp
int p = poll(&fds[0], fds_size, 1000);
```

Cela bloque jusqu'à 1000ms, puis retourne. On parcourt le vecteur et on agit uniquement sur les entrées où `revents != 0`.

---

## 4. Un seul poll() — accept + lecture/écriture

Oui, **un seul `poll()`**, un seul vecteur, trois catégories de fd toutes enregistrées ensemble avant chaque appel :

| Type de fd | Événements enregistrés |
|---|---|
| Sockets d'écoute du serveur | `POLLIN` |
| Sockets client | `POLLIN` / `POLLOUT` (depuis `get_poll_events()`) |
| Pipe stdout du CGI (`cgi_ut_fd_`) | `POLLIN` |
| Pipe stdin du CGI (`cgi_in_fd_`) | `POLLOUT` |

`accept()` est appelé quand un fd serveur se déclenche. `on_read()` / `on_writ()` sont appelés quand un fd client se déclenche. Les gestionnaires CGI sont appelés quand un fd de pipe se déclenche. Tout est distribué depuis la même boucle après le `poll()`.

L'exigence de **non-bloquant** concerne les **opérations d'E/S sur les sockets et les pipes**, pas `poll()` lui-même. On satisfait cette exigence en appelant :

```cpp
fcntl(client_fd, F_SETFL, O_NONBLOCK);
```

sur chaque fd client après `accept()`. C'est ce qui rend les E/S non-bloquantes.

`poll()` **a le droit de bloquer** — c'est littéralement son rôle. Il suspend le processus efficacement jusqu'à ce que quelque chose soit prêt, ce qui est bien meilleur qu'une boucle active (busy-loop). Le timeout de 1000ms est également intentionnel et correct dans ton cas : il garantit que la boucle principale se réveille périodiquement même si aucune E/S ne se déclenche, ce qui permet de gérer :

- Les timeouts client (vérification `CLIENT_TIMEOUT` en haut de la boucle)
- Les timeouts CGI (`cgi_.check_timeout()`)
- La vérification `Signals::should_stop()`

Si `poll()` avait `timeout = -1` (bloquer indéfiniment), ces vérifications basées sur le temps ne s'exécuteraient jamais tant qu'aucun événement d'E/S ne survient. Le timeout de 1000ms est un choix de conception délibéré, pas une violation.

**Ce que la contrainte signifie :** ne pas faire de `recv()`/`send()` bloquants directement sur les sockets — passer toutes les E/S par `poll()` et utiliser des fd non-bloquants. Ce qui est exactement ce que tu fais. ✅

---

## 5. poll() dans la boucle principale — lecture ET écriture simultanées

La boucle `while (!Signals::should_stop())` appelle `poll()` une fois par itération. Les fd client sont enregistrés avec `POLLIN` et `POLLOUT` selon le cas via `get_poll_events()`, et le dispatch vérifie les deux à chaque passage :

```cpp
if (fds[i].revents & POLLIN)  { ... on_read() ... }
else
if (fds[i].revents & POLLOUT) { ... on_writ() ... }
```

✅ Les deux directions sont vérifiées simultanément, à chaque itération.

---

## 6. Chemin du code, de poll() jusqu'aux E/S client

**Chemin de lecture client :**
1. `poll()` retourne — `revents & POLLIN` sur un fd client
2. `server_fd_port_.find(fd)` → pas trouvé
3. `cgi_fd_map.find(fd)` → pas trouvé
4. `clients.find(fd)` → trouvé
5. `fds[i].revents & POLLIN` → `it->second->on_read()` → `recv()` appelé à l'intérieur

**Chemin de sortie CGI :**
1. `poll()` retourne — `revents & (POLLIN | POLLHUP)` sur `cgi_ut_fd_`
2. `cgi_fd_map.find(fd)` → trouvé
3. `client.cgi_.on_stdut_ready(client)` → `read()` sur le pipe appelé à l'intérieur

**Aucun appel d'E/S n'existe en dehors de ce dispatch conditionné par poll().** ✅

---

## 7 & 8. Gestion des erreurs sur read/recv/write/send — à la fois -1 et 0

Dans `on_read()`, la valeur de retour de `recv()` est vérifiée pour les deux cas et mappée vers des états explicites :

- `n == 0` → `READ_CLOSE` (le pair a fermé la connexion) → `con_manager.rmv_client(fd)` ✅
- `n < 0` → `READ_ERROR` → `con_manager.rmv_client(fd)` ✅

Même logique dans `on_writ()` pour `send()` :

- fatal/erreur → `WRIT_FATAL` → `con_manager.rmv_client(fd)` ✅

Ne vérifier qu'un seul cas parmi `-1` ou `0` n'est pas suffisant — on gère les deux. ✅

---

## 9. errno après read/write — PAS utilisé pour le contrôle de flux

Tout le branchement après `recv()`/`send()` est piloté par des états de valeur de retour (`READ_ERROR`, `READ_CLOSE`, `WRIT_FATAL`, `BUILD_FATAL_ERR`). La seule vérification d'`errno` à l'intérieur de `run()` se trouve après `accept()`, qui n'est ni une lecture ni une écriture et est explicitement exempté de cette règle. ✅

---

## 10. Aucune E/S sur les fd événementiels sans passer par poll()

Chaque `recv()`/`send()` sur un socket et chaque `read()`/`write()` sur un pipe CGI n'est atteint que via le bloc de dispatch situé après le `poll()`. Il n'existe aucun chemin de code qui appelle une E/S sur un socket ou un pipe sans que `poll()` n'ait d'abord indiqué que ce fd était prêt. ✅

---

## 11. Les fichiers disque ne bloquent pas la boucle événementielle

Le parsing de la configuration se fait une seule fois au démarrage, avant la boucle. La lecture des fichiers statiques utilise `open()`/`read()` sur des fichiers disque classiques pendant le traitement des requêtes — ceux-ci ne sont **pas** enregistrés dans le vecteur `pollfd` et ne passent pas par `poll()`. C'est explicitement autorisé : le noyau considère toujours les fichiers réguliers comme prêts, ils ne peuvent donc jamais bloquer la boucle événementielle. ✅

---

## 12. Compilation — pas de relink

```bash
make        # build complet
make        # second lancement → rien ne recompile ni ne relink
```

Toutes les dépendances des fichiers objets sont correctement suivies dans le Makefile. ✅

---

