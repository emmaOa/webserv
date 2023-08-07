# serve mp4 file
curl -s --output test0.mp4 localhost:8001/toto/video30.mp4
diff test0.mp4 ./public/video30.mp4
rm test0.mp4

# serve txt file 
curl -s --output test4.txt localhost:8001/toto/txt.txt
diff test4.txt ./public/txt.txt
rm test4.txt

# serve pdf file 
curl -s --output test1.pdf localhost:8001/toto/book.pdf
diff test1.pdf ./public/book.pdf
rm test1.pdf

# # serve gif file
# curl -s --output test2.gif localhost:8001/toto/gunsNRoses.gif
# diff test2.gif ./public/gunsNRoses.gif
# rm test2.gif 
