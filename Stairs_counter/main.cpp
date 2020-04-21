#include<opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"  
#include "opencv2/highgui/highgui.hpp"  
#include <opencv2/imgcodecs.hpp>
#include<iostream>
#include "img_processing.h"

using namespace std;
using namespace cv;

int main(){
	vector<vector<int>> mask = {
	{ 0 ,  0 ,  0},
	{ 0 , -1 ,  0},
	{ 0 ,  1 ,  0} };

	Mat img = imread("C:/Users/lilia/github/Stairs_Steps_Detection/Stairs/s9.jpg", IMREAD_GRAYSCALE);
	//Si l'image ne charge pas on retourne une erreur
	if (!img.data)
	{
		cout << "Image not loaded";
		return -1;
	}
	cout << "Image loaded\n";
	//Création de l'objet
	img_processing i;
	//Affichage des informations sur les niveaux de gris de l'image 
	i.hist_info(img);

	//Etape 1 : Application du filtre passe bas deux fois et affichage de l'image obtenue
	Mat lpf = i.LPF(img);
	lpf = i.LPF(lpf);
	imshow("1", lpf);
	
	//Application d'un filtre qui extrait les contours horizontaux 
	Mat hpf = i.LPF(lpf,mask);
	imshow("2", hpf);

	//Application d'un filtre median et affichage de l'image obtenue
	medianBlur(hpf, hpf, 3);
	imshow("3", hpf);

	//Application d'un filtre qui extrait les contours horizontaux 
	hpf = i.LPF(hpf, mask);
	imshow("4", hpf);

	//Extraction des lignes horizontales
	Mat horizontal = hpf.clone();
	int horizontal_size = horizontal.cols / 30;
	Mat horizontalStructure = getStructuringElement(MORPH_RECT, Size(horizontal_size, 1));
	//Morph Operations
	
	erode(horizontal, horizontal, horizontalStructure, Point(-1, -1));
	dilate(horizontal, horizontal, horizontalStructure, Point(-1, -1));

	//Affichage du résultat
	imshow("horizontal", horizontal);
	
	//Binarisation de l'image
	Mat bin = i.Binary(horizontal, 50);
	imshow("Binary", bin);
	
	//Test des deux compteurs 
	cout << "C2 Moyenne : "<<i.count_2(bin) << endl;
	cout << "C1 Max : " <<i.count_1(bin) << endl;
	waitKey(0);




	return 0;
}