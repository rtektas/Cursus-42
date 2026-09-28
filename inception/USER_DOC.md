# User Documentation - Inception

This documentation explains how to use the Inception Docker infrastructure from an end-user or administrator perspective.

## Overview of Services

The Inception project provides three main services:

1. **WordPress Website** - A complete WordPress installation accessible via HTTPS
2. **Web Server (Nginx)** - Handles all HTTP/HTTPS traffic and serves the website
3. **Database (MariaDB)** - Stores all WordPress data, user information, and content

## Quick Start

### Starting the Project

To start the entire infrastructure:

```bash
cd /path/to/inception
make up
```

This command will:
- Create Docker volumes for persistent data storage
- Build all Docker images
- Start all three containers (Nginx, WordPress, MariaDB)
- Set up networking between containers

Wait 30-60 seconds for all services to fully initialize.

### Accessing the Website

1. **Configure your hosts file** (if not already done):
   ```bash
   # On Linux/Mac, edit /etc/hosts
   # On Windows, edit C:\Windows\System32\drivers\etc\hosts
   # Add this line:
   127.0.0.1 rtektas.42.fr
   ```

2. **Open your browser** and navigate to:
   - **Public Website**: https://rtektas.42.fr
   - **Admin Dashboard**: https://rtektas.42.fr/wp-admin

3. **Accept the SSL warning** - The certificate is self-signed and won't be trusted by browsers, which is normal.

### Logging In to WordPress

Credentials are not stored in the repository. Copy `srcs/.env.example` to `srcs/.env` and fill in real values locally (or use files under `secrets/`, which must be gitignored).

Example placeholders:
- Administrator username: `inception_owner`; password: `secrets/wp_admin_password.txt`
- Regular username: `author42`; password: `secrets/wp_user_password.txt`

## Stopping and Managing the Project

### Stop All Containers

To stop the infrastructure without removing data:

```bash
make down
```

Your data will be preserved in the volumes and will be available when you restart.

### View Container Status

```bash
make ps
```

This shows whether each container (Nginx, WordPress, MariaDB) is running.

### View Logs

To see what's happening in the containers:

```bash
make logs
```

Or for a specific service:

```bash
docker compose -f srcs/docker-compose.yml logs nginx
docker compose -f srcs/docker-compose.yml logs wordpress
docker compose -f srcs/docker-compose.yml logs mariadb
```

### Full Cleanup

If you want to start fresh and remove all data:

```bash
make clean      # Removes containers and volumes
make fclean     # Also removes Docker images
```

## Managing Credentials

### Where Credentials Are Stored

Passwords are generated locally by `make prepare` and stored in the gitignored `secrets/` directory:

```
secrets/
├── wp_admin_password.txt     # WordPress administrator password
│   ├── wp_user_password.txt      # WordPress regular-user password
├── db_password.txt         # Database user password
├── db_root_password.txt    # Database root password
│   └── wp_user_password.txt      # WordPress regular-user password
```

### Viewing Current Credentials

```bash
cat secrets/db_root_password.txt
cat secrets/db_password.txt
cat secrets/wp_admin_password.txt
cat secrets/wp_user_password.txt
```

### Changing Credentials

To change credentials, you'll need to update:
1. Update the appropriate file in `secrets/`
2. Remove the persistent data with `make fclean` if you want the initialization scripts to recreate the accounts
3. Run `make up`

## Common Tasks

### Reset Everything to Default

```bash
make fclean
make up
```

This will:
- Remove all containers, images, and volumes
- Rebuild from scratch
- Initialize a fresh WordPress installation

### Backup Your Website Data

Your data is already stored in persistent volumes at:
- `~/data/wordpress/` - WordPress files and uploads
- `~/data/mariadb/` - Database files

To manually backup:

```bash
tar -czf wordpress-backup.tar.gz ~/data/wordpress/
tar -czf database-backup.tar.gz ~/data/mariadb/
```

### Check Service Health

To verify all services are running correctly:

```bash
docker compose -f srcs/docker-compose.yml ps
```

Expected output should show all three containers as "healthy" or "up".

## Troubleshooting

### Nginx Container Won't Start

1. Check if port 443 is already in use:
   ```bash
   sudo lsof -i :443
   ```

2. Check Nginx logs:
   ```bash
   docker compose -f srcs/docker-compose.yml logs nginx
   ```

### WordPress Won't Load

1. Verify MariaDB is running:
   ```bash
   docker compose -f srcs/docker-compose.yml logs mariadb
   ```

2. Check WordPress logs:
   ```bash
   docker compose -f srcs/docker-compose.yml logs wordpress
   ```

### Database Connection Errors

1. Verify that `secrets/db_password.txt` exists and is non-empty
2. Restart the WordPress container:
   ```bash
   docker compose -f srcs/docker-compose.yml restart wordpress
   ```

### Can't Access https://rtektas.42.fr

1. Verify `/etc/hosts` entry:
   ```bash
   cat /etc/hosts | grep rtektas
   ```

2. Clear browser cache and try again with a private/incognito window

3. Check if Nginx is running:
   ```bash
   docker compose -f srcs/docker-compose.yml ps nginx
   ```

## Best Practices

1. **Regular Backups**: Periodically backup your `~/data/` directories
2. **Update WordPress**: Keep WordPress, plugins, and themes updated through the admin dashboard
3. **Strong Passwords**: Change default credentials in `secrets/` before production use
4. **Monitor Logs**: Regularly check container logs for errors or issues
5. **Test Restart**: Periodically test that `make down` and `make up` work smoothly

## Performance Notes

- First startup typically takes 30-60 seconds as services initialize
- Subsequent startups are much faster (5-10 seconds)
- Database initialization only happens on first run; subsequent runs use existing data

## Additional Resources

For more information, see:
- [README.md](README.md) - Project overview and architecture
- [DEV_DOC.md](DEV_DOC.md) - Developer setup and advanced configuration
