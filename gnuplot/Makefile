IMAGES= images/output.png

all : $(IMAGES)

clean:
	rm $(IMAGES)

images/output.png : ploting.gpi data/wallis.txt data/leibniz.txt
	gnuplot ploting.gpi
