#!/bin/bash
set -euo pipefail

MYSQL_PASSWORD="$(cat /run/secrets/db_password)"
MYSQL_ROOT_PASSWORD="$(cat /run/secrets/db_root_password)"

mkdir -p /run/mysqld /var/lib/mysql
chown -R mysql:mysql /run/mysqld /var/lib/mysql

if [ ! -d /var/lib/mysql/mysql ]; then
    mariadb-install-db --user=mysql --datadir=/var/lib/mysql >/dev/null

    mariadbd --user=mysql --datadir=/var/lib/mysql \
        --socket=/run/mysqld/mysqld.sock \
        --pid-file=/run/mysqld/mysqld.pid \
        --skip-networking &
    temp_pid="$!"

    until mariadb-admin --socket=/run/mysqld/mysqld.sock ping --silent; do
        sleep 1
    done

    mariadb --socket=/run/mysqld/mysqld.sock <<SQL
ALTER USER 'root'@'localhost' IDENTIFIED BY '${MYSQL_ROOT_PASSWORD}';
CREATE DATABASE IF NOT EXISTS \`${MYSQL_DATABASE}\`;
CREATE USER IF NOT EXISTS '${MYSQL_USER}'@'%' IDENTIFIED BY '${MYSQL_PASSWORD}';
ALTER USER '${MYSQL_USER}'@'%' IDENTIFIED BY '${MYSQL_PASSWORD}';
GRANT ALL PRIVILEGES ON \`${MYSQL_DATABASE}\`.* TO '${MYSQL_USER}'@'%';
FLUSH PRIVILEGES;
SQL

    mariadb-admin --socket=/run/mysqld/mysqld.sock \
        -uroot -p"${MYSQL_ROOT_PASSWORD}" shutdown
    wait "${temp_pid}"
fi

exec mariadbd --user=mysql --datadir=/var/lib/mysql \
    --bind-address=0.0.0.0 \
    --socket=/run/mysqld/mysqld.sock \
    --pid-file=/run/mysqld/mysqld.pid
