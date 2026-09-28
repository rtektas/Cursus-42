# Developer Documentation - Inception

This documentation is for developers who want to understand, modify, or extend the Inception Docker infrastructure.

## Environment Setup

### Prerequisites

- Linux or macOS (WSL2 on Windows)
- Docker Desktop or Docker Engine >= 20.10
- Docker Compose >= 2.0
- Git
- Text editor (VS Code, Vim, etc.)

### Initial Setup

1. **Clone the repository**:
   ```bash
   git clone <repository_url>
   cd inception
   ```

2. **Create the secrets directory** (contains credentials):
   ```bash
   mkdir -p secrets
   # Add password files (see below)
   ```

3. **Configure environment variables**:
   ```bash
   # Edit or create srcs/.env
   nano srcs/.env
   ```

4. **Verify Docker installation**:
   ```bash
   docker --version
   docker compose version
   ```

## Project Structure

```
inception/
├── Makefile                    # Build automation
├── README.md                   # User-facing documentation
├── USER_DOC.md                 # End-user guide
├── DEV_DOC.md                  # This file
├── secrets/                    # Sensitive credentials (gitignored)
│   ├── credentials.txt
│   ├── db_password.txt
│   └── db_root_password.txt
└── srcs/
    ├── .env                    # Environment configuration
    ├── docker-compose.yml      # Docker orchestration
    ├── check.sh                # Verification script
    ├── tests/
    │   └── test_service.sh     # Service tests
    └── requirements/
        ├── nginx/
        │   ├── Dockerfile      # Nginx image definition
        │   ├── .dockerignore
        │   ├── conf/
        │   │   └── nginx.conf   # Nginx configuration
        │   └── tools/           # Helper scripts
        ├── wordpress/
        │   ├── Dockerfile      # WordPress+PHP-FPM image
        │   ├── .dockerignore
        │   ├── conf/
        │   │   └── www.conf     # PHP-FPM configuration
        │   └── tools/
        │       └── init_wordpress.sh  # Initialization script
        └── mariadb/
            ├── Dockerfile      # MariaDB image definition
            ├── .dockerignore
            ├── conf/           # Configuration files
            └── tools/
                └── init_mariadb.sh    # Database initialization
```

## Building and Running

### Build the Entire Stack

```bash
make up
```

This:
1. Creates necessary data directories
2. Builds Docker images from Dockerfiles
3. Creates Docker network
4. Starts all containers
5. Initializes the database and WordPress

### Run Individual Services

Build only one service:
```bash
docker compose -f srcs/docker-compose.yml build nginx
docker compose -f srcs/docker-compose.yml build wordpress
docker compose -f srcs/docker-compose.yml build mariadb
```

### Start/Stop Services

```bash
# Start all services
docker compose -f srcs/docker-compose.yml up -d

# Stop all services
docker compose -f srcs/docker-compose.yml down

# Restart a specific service
docker compose -f srcs/docker-compose.yml restart wordpress

# View logs
docker compose -f srcs/docker-compose.yml logs -f wordpress
```

## Managing Containers and Volumes

### Container Management

```bash
# List running containers
docker ps

# List all containers (including stopped)
docker ps -a

# Execute command in container
docker exec -it wordpress bash

# View container resource usage
docker stats

# Inspect container configuration
docker inspect nginx
```

### Volume Management

```bash
# List all volumes
docker volume ls

# Inspect a specific volume
docker volume inspect mariadb_data

# Remove unused volumes
docker volume prune

# Backup volume data
docker run --rm -v mariadb_data:/data -v $(pwd):/backup alpine tar czf /backup/mariadb_backup.tar.gz /data

# View volume contents
docker run --rm -v mariadb_data:/data alpine ls -la /data
```

### Data Location

Volumes store data at:
- **WordPress files**: `~/data/wordpress/`
- **Database files**: `~/data/mariadb/`

## Configuration Files

### .env File

Located at `srcs/.env`. Do NOT commit real credentials. Use `srcs/.env.example` as a template and copy it locally:

```bash
# Copy template to create local .env (do not commit):
cp srcs/.env.example srcs/.env
# then edit srcs/.env with real credentials
```

The example file contains placeholders for the database and WordPress credentials. Keep sensitive values in local files or a secure secret store.

### nginx.conf

Web server configuration handling:
- HTTPS on port 443
- TLS 1.2 and 1.3 only
- Reverse proxy to WordPress+PHP-FPM
- Static file caching

### PHP-FPM Configuration (www.conf)

- Worker processes: dynamic with min/max configuration
- Socket communication with Nginx
- User/group: www-data

### MariaDB Initialization

`init_mariadb.sh` creates:
- Database specified in `MYSQL_DATABASE`
- User with credentials from `MYSQL_USER` and `MYSQL_PASSWORD`
- Root password set to `MYSQL_ROOT_PASSWORD`

### WordPress Initialization

`init_wordpress.sh`:
- Downloads WordPress core using WP-CLI
- Creates wp-config.php with database credentials
- Installs WordPress with admin account
- Creates additional user account

## Common Development Tasks

### Modify Nginx Configuration

1. Edit `srcs/requirements/nginx/conf/nginx.conf`
2. Test syntax:
   ```bash
   docker exec nginx nginx -t
   ```
3. Reload configuration:
   ```bash
   docker exec nginx nginx -s reload
   ```

### Add PHP Extension

1. Edit `srcs/requirements/wordpress/Dockerfile`
2. Add extension installation:
   ```dockerfile
   RUN apt-get install -y php8.2-gd php8.2-xml
   ```
3. Rebuild and restart:
   ```bash
   make down
   make up
   ```

### Access Database

```bash
# Connect to MariaDB
docker exec -it mariadb mysql -u root -p

# Query from host
docker exec mariadb mysql -u wpuser -p wordpress -e "SELECT * FROM wp_users;"

# Dump database
docker exec mariadb mysqldump -u root -p wordpress > backup.sql
```

### Debug WordPress

Enable debug mode by adding to `srcs/requirements/wordpress/tools/init_wordpress.sh`:

```bash
wp config set WP_DEBUG true --allow-root
wp config set WP_DEBUG_LOG true --allow-root
```

Then check logs at `/var/www/html/wp-content/debug.log`

## Testing

### Run Test Suite

```bash
make test
```

This verifies:
- All containers are running
- Docker network exists
- Volumes are created
- HTTPS is accessible
- Services are responding to health checks

### Manual Testing

```bash
# Test HTTPS connectivity
curl -k https://rtektas.42.fr/

# Test database connection
docker exec wordpress mysql -h mariadb -u wpuser -p wordpress -e "SELECT 1"

# Check Nginx reverse proxy
curl -v https://rtektas.42.fr/ 2>&1 | grep -i "WordPress"
```

## Docker Image Details

### Nginx Image

- **Base**: debian:bullseye
- **Packages**: nginx, openssl
- **Port**: 443 (HTTPS)
- **Certificate**: Self-signed, auto-generated on build
- **Entrypoint**: `nginx -g daemon off;`

### WordPress Image

- **Base**: debian:bullseye
- **Packages**: php8.2, php8.2-fpm, php8.2-mysql, mariadb-client, curl
- **Tools**: WP-CLI for WordPress management
- **Port**: 9000 (PHP-FPM socket)
- **Entrypoint**: init_wordpress.sh

### MariaDB Image

- **Base**: debian:bullseye
- **Package**: mariadb-server
- **Port**: 3306
- **Entrypoint**: init_mariadb.sh

## Security Considerations

1. **Credentials**: Never commit `secrets/` directory or `.env` file with real passwords
2. **SSL/TLS**: Self-signed certificates are for development; use proper CA-signed certs for production
3. **Network**: Services communicate through private Docker network; only Nginx exposes port 443
4. **Health Checks**: Enable monitoring to detect and auto-restart failed containers
5. **Updates**: Keep base images (debian:bullseye) updated for security patches

## Troubleshooting

### Container Won't Start

```bash
# Check logs
docker logs <container_name>

# Rebuild image
docker compose -f srcs/docker-compose.yml build --no-cache <service>

# Reset everything
make fclean && make up
```

### Port Already in Use

```bash
# Find what's using port 443
sudo lsof -i :443

# Kill the process
sudo kill -9 <PID>

# Or use a different port in docker-compose.yml
```

### Volume Permission Errors

```bash
# Check volume permissions
ls -la ~/data/wordpress/
ls -la ~/data/mariadb/

# Reset permissions
sudo chown -R 33:33 ~/data/wordpress/   # www-data user in container
sudo chown -R 999:999 ~/data/mariadb/   # mysql user in container
```

### DNS Resolution Issues

```bash
# Verify Docker network
docker network ls
docker network inspect inception_network

# Test inter-container communication
docker exec wordpress ping mariadb
```

## Performance Optimization

1. **Layer Caching**: Order Dockerfile commands from most stable to least stable
2. **Multi-stage Builds**: Keep images lean by removing build dependencies
3. **Health Checks**: Quick checks (not heavy operations) ensure fast startup
4. **Volume Drivers**: Named volumes are optimized for container performance

## Contributing and Git Workflow

1. Never commit credentials (use `.gitignore`)
2. Commit Dockerfiles and configurations
3. Document changes in README
4. Test thoroughly before pushing
5. Use meaningful commit messages

## Additional Resources

- [Docker Documentation](https://docs.docker.com/)
- [Docker Compose Specification](https://compose-spec.io/)
- [Nginx Documentation](https://nginx.org/en/docs/)
- [WordPress Development](https://developer.wordpress.org/)
- [MariaDB Administration](https://mariadb.com/kb/en/administration/)

## Quick Reference

```bash
# Essential commands
make up                    # Start everything
make down                  # Stop everything
make clean                 # Stop and remove volumes
make fclean                # Full cleanup including images
make ps                    # Check status
make logs                  # View logs
make re                    # Rebuild from scratch

# Docker Compose equivalents
docker compose -f srcs/docker-compose.yml ps
docker compose -f srcs/docker-compose.yml logs -f
docker compose -f srcs/docker-compose.yml exec <service> <command>
docker compose -f srcs/docker-compose.yml build --no-cache
```
