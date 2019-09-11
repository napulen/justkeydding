FROM ubuntu:16.04

RUN apt-get update
RUN apt-get install -y build-essential libboost-all-dev libsndfile1-dev vamp-plugin-sdk

COPY . /justkeydding/
WORKDIR /justkeydding

RUN make

ENTRYPOINT ["bin/justkeydding"]
