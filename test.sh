#!/usr/bin bash

for query in n1 n3 n5 n8 s1 s3 s5 s8
do
timeout 60 ./build/main/program ./data/lcc_yeast.igraph ./query/lcc_yeast_$query.igraph ./cs/cs_yeast_$query.igraph > ./embd/embd_yeast_$query.txt;
done

for query in n1 n3 n5 n8 s1 s3 s5 s8
do
timeout 60 ./build/main/program ./data/lcc_human.igraph ./query/lcc_human_$query.igraph ./cs/cs_human_$query.igraph > ./embd/embd_human_$query.txt;
done

for query in n1 n3 n5 n8 s1 s3 s5 s8
do
timeout 60 ./build/main/program ./data/lcc_hprd.igraph ./query/lcc_hprd_$query.igraph ./cs/cs_hprd_$query.igraph > ./embd/embd_hprd_$query.txt;
done