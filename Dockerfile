FROM php:7.4-apache
ADD . /extension
WORKDIR /extension
RUN apt-get update && apt-get install -y \
    time
RUN phpize && ./configure && make && make install
RUN docker-php-ext-enable trutimeout.so
ADD index.php /var/www/html/
RUN echo "StartServers 5" >> /etc/apache2/apache2.conf && \
    echo "MinSpareServers 5" >> /etc/apache2/apache2.conf && \
    echo "MaxSpareServers 5" >> /etc/apache2/apache2.conf && \
    echo "MaxClients 5" >> /etc/apache2/apache2.conf
ENTRYPOINT [ "php"]
CMD ["/extension/test.php"]