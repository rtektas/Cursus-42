#!/bin/bash
set -euo pipefail

COMPOSE="docker compose -f srcs/docker-compose.yml"

echo "=== Containers ==="
$COMPOSE ps

echo "=== HTTPS ==="
curl -kfsSI https://rtektas.42.fr | head -n 1

echo "=== TLS versions ==="
openssl s_client -connect rtektas.42.fr:443 -tls1_2 </dev/null >/dev/null 2>&1
echo "TLS 1.2: OK"
if openssl s_client -connect rtektas.42.fr:443 -tls1 </dev/null >/dev/null 2>&1; then
    echo "TLS 1.0 unexpectedly accepted"
    exit 1
fi
echo "TLS 1.0: rejected"

echo "=== WordPress ==="
docker exec wordpress wp core is-installed --allow-root
docker exec wordpress wp user list --allow-root

echo "=== MariaDB ==="
docker exec mariadb mariadb-admin ping -uroot -p"$(cat secrets/db_root_password.txt)" --silent

echo "=== Volumes ==="
docker volume inspect mariadb_data wordpress_data --format '{{.Name}} -> {{.Options.device}}'
