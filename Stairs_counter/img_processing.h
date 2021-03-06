#include<opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include <opencv2/imgcodecs.hpp>
#include <math.h> 
#include <cmath> 
#include<iostream>
#include <algorithm>
#include <iterator>
using namespace std;
using namespace cv;

class img_processing {
	public:
		img_processing(){}
		//Return Max Min Mode Mean SD de l'image
		vector<float> hist_info(Mat img);
		//Transforme l'image en image binaire avec en param�tre le seuil
		Mat Binary(Mat img,int threshold=120);
		//Suppression des premiers et derni�rres ranges 
		Mat Del_rows(Mat img);
		//Application d'un filtre passe bas/moyenneur 
		Mat LPF(Mat img,vector<vector<int>> filter = { {1,1,1},{1,1,1},{1,1,1} });
		//Suppression du centre de la DFT (Inutile)
		//Mat DC_DFT(Mat img);
		//Counter 1 : prend le plus grand nombre de variation align�s sur une colone de pixels 
		int count_lines(Mat img);
		//Counter 2 : prend la variation globale de l'image avec la moyenne des rang�es
		int count_mean(Mat img);
	private:
};