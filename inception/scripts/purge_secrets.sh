#!/usr/bin/env bash
set -euo pipefail

REPO_ROOT="$(pwd)"

if [ ! -d .git ]; then
  echo "Erreur: ce dossier ne semble pas être une repo Git. Exécute depuis la racine du dépôt."
  exit 1
fi

if [ -n "$(git status --porcelain)" ]; then
  echo "L'arbre de travail contient des modifications non commit. Commit ou stash avant de continuer." >&2
  git status --porcelain
  exit 1
fi

echo "Création d'une branche de sauvegarde 'backup-before-secrets'..."
git branch -f backup-before-secrets

if command -v git-filter-repo >/dev/null 2>&1; then
  FILTER_TOOL=git-filter-repo
else
  echo "git-filter-repo introuvable, tentative d'installation via pip (user)..."
  if python3 -m pip install --user git-filter-repo; then
    export PATH="$HOME/.local/bin:$PATH"
    if ! command -v git-filter-repo >/dev/null 2>&1; then
      echo "Installation échouée. Installe git-filter-repo manuellement et relance." >&2
      exit 1
    fi
    FILTER_TOOL=git-filter-repo
  else
    echo "Impossible d'installer git-filter-repo automatiquement. Installe 'git-filter-repo' ou 'bfg' et relance." >&2
    exit 1
  fi
fi

echo "Purge des chemins sensibles (secrets/ et srcs/.env) de l'historique..."
# Supprime du repo l'arborescence secrets et le fichier srcs/.env
git filter-repo --invert-paths --path secrets --path srcs/.env

echo "Nettoyage final (reflog+gc)..."
git reflog expire --expire=now --all || true
git gc --prune=now --aggressive || true

echo "Vérification rapide pour motifs sensibles..."
grep -R --line-number -E "wppass|rootpass|adminpass|userpass|MYSQL_PASSWORD|WP_ADMIN_PASSWORD" || true

echo "Opération terminée. Si tout est OK, poussez en force vers le remote si besoin :"
echo "  git push --force --all"
echo "  git push --force --tags"

echo "Note: cette opération réécrit l'historique. Assure-toi que les collaborateurs savent qu'ils devront recloner ou reseter leurs branches." 
