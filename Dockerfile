FROM ahmad88me/tadaswift:latest

WORKDIR /app

RUN mkdir -p bin
RUN mkdir -p build
COPY *.tsv /app/
COPY *.csv /app/
COPY scripts /app/scripts
COPY src /app/src
COPY Makefile /app/Makefile
COPY .git /app/.git


