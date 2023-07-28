# serve mp4 file
curl -s --output test0.mp4 localhost:8001/toto/video_1_hour.mp4
diff test0.mp4 ./public/video_1_hour.mp4
rm test0.mp4

# serve jpeg file 
curl -s --output test1.jpeg localhost:8001/toto/image.jpeg
diff test1.jpeg ./public/image.jpeg
rm test1.jpeg

# serve gif file
curl -s --output test2.gif localhost:8001/toto/gunsNRoses.gif
diff test2.gif ./public/gunsNRoses.gif
rm test2.gif 

# serve pdf file 
curl -s --output test3.pdf localhost:8001/toto/pageDeCorrection.pdf
diff test3.pdf ./public/pageDeCorrection.pdf
rm test3.pdf
