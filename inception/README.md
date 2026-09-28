*This project has been created as part of the 42 curriculum by rtektas.*

## Description

This project sets up a complete Docker-based infrastructure for running a WordPress website with Nginx as a reverse proxy and MariaDB as the database backend. The goal is to gain practical experience in system administration and Docker orchestration by building a multi-container application from scratch.

The stack consists of three main services:
- **Nginx**: Acts as the reverse proxy and SSL/TLS termination point for HTTPS connections
- **WordPress + PHP-FPM**: Runs the WordPress application with PHP-FPM for dynamic content processing
- **MariaDB**: Provides the database backend for WordPress with persistent storage

## Instructions

### Prerequisites
- Docker and Docker Compose installed
- Virtual Machine or compatible Linux environment
- Network access to localhost

### Installation and Running

1. Clone the repository to your machine
2. Configure your local domain name to point to your IP address:
   ```bash
   # Add to /etc/hosts
   127.0.0.1 rtektas.42.fr
   ```

3. Build and start the entire infrastructure:
   ```bash
   make up
   ```

4. Access the website:
   - **Website**: https://rtektas.42.fr
   - **Admin Panel**: https://rtektas.42.fr/wp-admin

5. Manage containers:
   ```bash
   make ps          # Check container status
   make logs        # View container logs
   make down        # Stop all containers
   make clean       # Stop containers and remove volumes
   make fclean      # Full cleanup including Docker images
   make re          # Rebuild everything from scratch
   ```

### Credentials

Passwords are generated locally by `make prepare` in the gitignored `secrets/` directory and mounted into the containers through Docker Compose secrets:

- `secrets/db_root_password.txt`
- `secrets/db_password.txt`
- `secrets/wp_admin_password.txt`
- `secrets/wp_user_password.txt`

No password is stored in `.env`, a Dockerfile, or the Git repository. Usernames, database names, email addresses, and the domain are kept in `srcs/.env`.

### Data Storage

- WordPress files are stored in named volume `wordpress_data` → `~/data/wordpress`
- Database files are stored in named volume `mariadb_data` → `~/data/mariadb`

## Project Description

### Architecture Overview

This project demonstrates a production-like Docker setup with three containerized services communicating through a private Docker network. Each service runs in isolation with its own responsibilities:

```
Internet (Port 443/HTTPS)
        ↓
    Nginx Container
        ↓
    Docker Network
    ↙          ↘
WordPress-PHP  MariaDB
Container      Container
```

### Key Design Choices

1. **Separate Containers**: Each service (Nginx, WordPress+PHP-FPM, MariaDB) runs in its own container, following the "single responsibility principle"

2. **Docker Network**: A custom Docker bridge network (`inception_network`) ensures secure inter-container communication without exposing ports unnecessarily

3. **Named Volumes**: Persistent data storage using Docker named volumes (not bind mounts) ensures data persistence across container restarts

4. **TLS/SSL**: Self-signed certificates provide HTTPS encryption, with TLSv1.2 and TLSv1.3 protocols only

5. **Health Checks**: Each container includes health checks to verify service availability and enable proper startup dependencies

### Comparison: Key Concepts

#### Virtual Machines vs Docker

| Aspect | Virtual Machines | Docker Containers |
|--------|------------------|------------------|
| **Overhead** | High (full OS per VM) | Low (shared kernel) |
| **Boot Time** | Slow (minutes) | Fast (seconds) |
| **Resource Usage** | Heavy (multiple GBs per VM) | Lightweight (MBs) |
| **Portability** | Limited to hypervisor | Highly portable |
| **Isolation** | Complete OS isolation | Process-level isolation |
| **Use Case** | Full OS separation needed | Application containerization |

**Why Docker for this project:** Docker is lightweight, portable, and ideal for microservices like WordPress. VMs would be overkill and resource-intensive.

#### Secrets vs Environment Variables

| Aspect | Environment Variables | Docker Secrets |
|--------|----------------------|-----------------|
| **Security** | Visible in container environment | Encrypted, only in memory |
| **Persistence** | Stored in .env files (risky) | Managed by Docker, removed from Git |
| **Use Case** | Non-sensitive config | Passwords, API keys, credentials |
| **Exposure Risk** | High (visible in process list) | Low (only available to service that needs it) |

**Our Approach:** 
- Non-sensitive configuration (domain, database names) → `.env` file
- Sensitive credentials (passwords) → `secrets/` directory (gitignored)
- Docker secrets feature can be enabled for production deployments

#### Docker Network vs Host Network

| Aspect | Docker Network | Host Network |
|--------|----------------|--------------|
| **Isolation** | Full network isolation | Container shares host network stack |
| **Inter-container Communication** | Through network with DNS | Direct, no overhead |
| **Port Mapping** | Explicit port binding | Direct port access |
| **Security** | High (isolated) | Low (exposed to host) |
| **Flexibility** | Containers can communicate easily | Limited multi-container support |

**Our Choice:** Custom bridge network provides isolation, security, and clear service communication without exposing internal ports.

#### Docker Volumes vs Bind Mounts

| Aspect | Docker Volumes | Bind Mounts |
|--------|----------------|------------|
| **Management** | Docker-managed | Host filesystem-managed |
| **Portability** | High (platform-independent) | Low (host path dependent) |
| **Permissions** | Docker handles permissions | User/OS permissions apply |
| **Performance** | Optimized for containers | Variable (depends on host FS) |
| **Use Case** | Production, multi-container sharing | Development, direct file access |
| **Subject Compliance** | ✅ Required and used | ❌ Explicitly forbidden |

**Our Implementation:** Named volumes ensure consistent data persistence across container restarts, platform independence, and compliance with the project requirements.

## Resources

### Docker Documentation
- [Docker Official Documentation](https://docs.docker.com/)
- [Docker Compose Reference](https://docs.docker.com/compose/compose-file/)
- [Best Practices for Writing Dockerfiles](https://docs.docker.com/develop/dev-best-practices/dockerfile_best-practices/)

### Nginx Configuration
- [Nginx Documentation](https://nginx.org/en/docs/)
- [Nginx SSL/TLS Configuration](https://nginx.org/en/docs/http/ngx_http_ssl_module.html)

### WordPress & PHP-FPM
- [WordPress Documentation](https://wordpress.org/documentation/)
- [PHP-FPM Official Guide](https://www.php.net/manual/en/install.fpm.php)
- [WP-CLI Documentation](https://developer.wordpress.org/cli/commands/)

### MariaDB
- [MariaDB Official Documentation](https://mariadb.com/kb/en/)
- [MariaDB Docker Hub](https://hub.docker.com/_/mariadb)

### System Administration
- [Linux File Permissions and Ownership](https://www.linux.com/training-tutorials/file-permissions-linux/)
- [Docker Security Best Practices](https://docs.docker.com/engine/security/)

### AI Usage

This project leveraged AI assistance for:

1. **Docker Dockerfile optimization** - AI helped refine multi-stage builds and layer optimization for efficient images
2. **Nginx configuration** - SSL/TLS setup and reverse proxy configuration best practices
3. **Bash scripting** - Init scripts for container startup and service initialization
4. **Documentation** - Structuring and clarifying technical explanations
5. **Troubleshooting** - Debug strategies for container networking and volume issues

AI was used to accelerate learning and reduce repetitive tasks, but all code was reviewed, understood, and validated for correctness and security compliance.