# serve mp4 file
curl -s --output test0.mp4 localhost:7001/toto/video.mp4
diff test0.mp4 ./public/video.mp4
rm test0.mp4

# serve txt file 
curl -s --output test4.txt localhost:7001/toto/txt.txt
diff test4.txt ./public/txt.txt
rm test4.txt

# serve pdf file 
curl -s --output test1.pdf localhost:7001/toto/web.pdf
diff test1.pdf ./public/web.pdf
rm test1.pdf

# serve gif file
curl -s --output test2.gif localhost:7001/toto/gunsNRoses.gif
diff test2.gif ./public/gunsNRoses.gif
rm test2.gif 

# serve jpeg file
curl -s --output test2.jpeg localhost:7001/toto/image.jpeg
diff test2.jpeg ./public/image.jpeg
rm test2.jpeg
