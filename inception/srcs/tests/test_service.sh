#!/bin/bash

GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"

echo "========== Inception Tests =========="

echo
echo "Checking containers..."

for container in mariadb wordpress nginx
do
	if docker ps --format "{{.Names}}" | grep -q "^${container}$"; then
		echo -e "${GREEN}[OK]${NC} $container is running"
	else
		echo -e "${RED}[KO]${NC} $container is not running"
	fi
done

echo
echo "Checking Docker network..."

if docker network ls --format "{{.Name}}" | grep -q "^inception_network$"; then
	echo -e "${GREEN}[OK]${NC} Docker network exists"
else
	echo -e "${RED}[KO]${NC} Docker network not found"
fi

echo
echo "Checking Docker volumes..."

for volume in mariadb_data wordpress_data
do
	if docker volume ls --format "{{.Name}}" | grep -q "^${volume}$"; then
		echo -e "${GREEN}[OK]${NC} $volume exists"
	else
		echo -e "${RED}[KO]${NC} $volume not found"
	fi
done

echo
echo "Checking HTTPS..."

if curl -k -s https://localhost > /dev/null; then
	echo -e "${GREEN}[OK]${NC} HTTPS is reachable"
else
	echo -e "${RED}[KO]${NC} HTTPS is not reachable"
fi

echo
echo "========== Tests finished =========="