FROM php:7.4-cli
ADD . /extension
WORKDIR /extension
RUN phpize && ./configure && make && make install
RUN docker-php-ext-enable trutimeout.so
ENTRYPOINT [ "php"]
CMD ["/extension/test.php"]