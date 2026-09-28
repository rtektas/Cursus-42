FROM debian:bookworm

RUN apt-get update && apt-get install -y \
	php8.2 \
	php8.2-fpm \
	php8.2-mysql \
	mariadb-client \
	curl \
	&& rm -rf /var/lib/apt/lists/*

RUN curl -O https://raw.githubusercontent.com/wp-cli/builds/gh-pages/phar/wp-cli.phar \
	&& chmod +x wp-cli.phar \
	&& mv wp-cli.phar /usr/local/bin/wp

COPY tools/init_wordpress.sh /init_wordpress.sh
COPY conf/www.conf /etc/php/8.2/fpm/pool.d/www.conf

RUN chmod +x /init_wordpress.sh

EXPOSE 9000

CMD ["/init_wordpress.sh"]