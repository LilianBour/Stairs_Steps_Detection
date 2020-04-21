#include "img_processing.h"

vector<float> img_processing::hist_info(Mat img) {
	float mean=0;
	int counter = 0;
	int max = 0;
	int min = 256;
	pair<int,int> count_mode = make_pair(-1,-1);
	vector<int> grey_values(256, 0);

	//Passage par tous les px pour calculer la moyenne, max, min and mode 
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			counter = counter + 1;
			mean = mean + (int)img.at<uchar>(i, j);
			if ((int)img.at<uchar>(i, j) < min) {
				min = (int)img.at<uchar>(i, j);
			}
			if ((int)img.at<uchar>(i, j) > max) {
				max = (int)img.at<uchar>(i, j);
			}

			grey_values[(int)img.at<uchar>(i, j)] = grey_values[(int)img.at<uchar>(i, j)] + 1;
			if (grey_values[(int)img.at<uchar>(i, j)] > count_mode.first) {
				count_mode.first = grey_values[(int)img.at<uchar>(i, j)];
				count_mode.second = (int)img.at<uchar>(i, j);
			}

		}
	}


	mean = mean / counter;
	//Calcule de l'écart type
	float sxm;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			sxm = ((int)img.at<uchar>(i, j) * mean) * ((int)img.at<uchar>(i, j) * mean);
		}
	}
	float sd = sqrt(sxm / (counter - 1));

	//Stockage des données
	img_processing::Max = max;
	img_processing::Min = min;
	img_processing::Mode = count_mode.second;
	img_processing::Mean = mean;
	img_processing::SD = sd;
	//Affichage des données
	cout << "Mean : " << mean << "\nSD : " << sd << "\nMode : " << count_mode.second << "\nMax : " << max << "\nMin : " << min << endl;
	//Renvoie des données
	return { (float)max,(float)min,(float)count_mode.second ,mean,sd };
}

Mat img_processing::Binary(Mat img, int threshold) {
	//Creation d'une image binary de la meme taille que l'image passée en paramètre
	Mat binary(img.rows, img.cols, CV_8UC1, Scalar(70));

	//Pour chaques pixels si c'est en dessous du seuil on met a 0 et si c'est au dessus ou égale au seuil on met a 255
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if ((int)img.at<uchar>(i, j) > threshold or (int)img.at<uchar>(i, j) == threshold ) {
				binary.at<uchar>(i, j) = 255;
			}
			if ((int)img.at<uchar>(i, j) < threshold) {
				binary.at<uchar>(i, j) = 0;
			}
		}
	}

	//Renvoie l'image binaire
	return binary;
}

Mat img_processing::LPF(Mat img, vector<vector<int>> filter) {
	//Création d'une image identique avec  bordures répliqués
	int border = 1;
	Mat img_lpf(img.rows+border*2, img.cols+border*2, CV_8UC1, Scalar(70));
	copyMakeBorder(img, img_lpf, border, border, border, border, BORDER_REPLICATE);

	//Application du filtre pour chaques pixels de l'image avec les bordures répliquées
	for (int i = 0+ trunc(filter.size() / 2); i < img_lpf.rows-trunc(filter.size()/2); i++) {
		for (int j = 0 + trunc(filter.size() / 2); j < img_lpf.cols - trunc(filter.size() / 2); j++) {
			//Application du filtre : 
			img_lpf.at<uchar>(i, j) =
			    (  (int)img_lpf.at<uchar>(i - 1, j - 1) * filter[0][0] + (int)img_lpf.at<uchar>(i - 1, j) * filter[0][1] + (int)img_lpf.at<uchar>(i - 1, j + 1) * filter[0][2]
				+ (int)img_lpf.at<uchar>(i    , j - 1) * filter[1][0] + (int)img_lpf.at<uchar>(i    , j) * filter[1][1] + (int)img_lpf.at<uchar>(i    , j + 1) * filter[1][2]
				+ (int)img_lpf.at<uchar>(i + 1, j - 1) * filter[2][0] + (int)img_lpf.at<uchar>(i + 1, j) * filter[2][1] + (int)img_lpf.at<uchar>(i + 1, j + 1) * filter[2][2]
				)/9;
		}
	}

	//Renvoie l'image sur laquelle le filtre a été appliqué 
	return img_lpf;
}


Mat img_processing::HPF(Mat img, vector<vector<int>> filter) {
	//Création d'une image identique avec  bordures répliqués
	int border = 1;
	Mat img_hpf(img.rows + border * 2, img.cols + border * 2, CV_8UC1, Scalar(70));
	copyMakeBorder(img, img_hpf, border, border, border, border, BORDER_REPLICATE);

	//Application du filtre pour chaques pixels de l'image avec les bordures répliquées
	for (int i = 0 + trunc(filter.size() / 2); i < img_hpf.rows - trunc(filter.size() / 2); i++) {
		for (int j = 0 + trunc(filter.size() / 2); j < img_hpf.cols - trunc(filter.size() / 2); j++) {
			//Application du filtre : 
			img_hpf.at<uchar>(i, j) =
				((int)img_hpf.at<uchar>(i - 1, j - 1) * filter[0][0]) / 9 + ((int)img_hpf.at<uchar>(i - 1, j) * filter[0][1]) / 9 + ((int)img_hpf.at<uchar>(i - 1, j + 1) * filter[0][2]) / 9
				+ ((int)img_hpf.at<uchar>(i, j - 1) * filter[1][0]) / 9 + ((int)img_hpf.at<uchar>(i, j) * filter[1][1]) / 9 + ((int)img_hpf.at<uchar>(i, j + 1) * filter[1][2]) / 9
				+ ((int)img_hpf.at<uchar>(i + 1, j - 1) * filter[2][0]) / 9 + ((int)img_hpf.at<uchar>(i + 1, j) * filter[2][1] + ((int)img_hpf.at<uchar>(i + 1, j + 1) * filter[2][2]) / 9);
		}
	}

	//Renvoie l'image sur laquelle le filtre a été appliqué 
	return img_hpf;
}

Mat img_processing::DC_DFT(Mat img) {
	//On prend la DFT de l'image en paramètre
	Mat originalFloat;
	img.convertTo(originalFloat, CV_32FC1, 1.0 / 255.0);
	Mat originalComplex[2] = { originalFloat,Mat::zeros(originalFloat.size(),CV_32F) };
	Mat dftReady;
	merge(originalComplex, 2, dftReady);
	Mat dftOfOriginal;
	dft(dftReady, dftOfOriginal, DFT_COMPLEX_OUTPUT);


	//Convertissage de la DFT pour pouvoir l'afficher
	Mat splitArray[2] = { Mat::zeros(dftOfOriginal.size(),CV_32F),Mat::zeros(dftOfOriginal.size(),CV_32F) };
	split(dftOfOriginal, splitArray);
	Mat dftMagnitude;
	magnitude(splitArray[0], splitArray[1], dftMagnitude);
	log(dftMagnitude, dftMagnitude);
	normalize(dftMagnitude, dftMagnitude, 0, 1, NORM_MINMAX);

	//Recentrage de la DFT convertit
	int centerX = dftMagnitude.cols / 2;
	int centerY = dftMagnitude.rows / 2;

	Mat q1(dftMagnitude, Rect(0, 0, centerX, centerY));
	Mat q2(dftMagnitude, Rect(centerX, 0, centerX, centerY));
	Mat q3(dftMagnitude, Rect(0, centerY, centerX, centerY));
	Mat q4(dftMagnitude, Rect(centerX, centerY, centerX, centerY));

	Mat swapMap;
	q1.copyTo(swapMap);
	q4.copyTo(q1);
	swapMap.copyTo(q4);
	q2.copyTo(swapMap);
	q3.copyTo(q2);
	swapMap.copyTo(q3);

	//Affichage de la DFT convertit et recentrée
	imshow("DFT", dftMagnitude);
	//waitKey(0);
	
	//Supression de lignes verticales de la DFT
	for (int i = 0; i < dftMagnitude.rows; i++) {
		for (int j = 0; j < dftMagnitude.cols; j++) {
			if (i > round(dftMagnitude.rows / 2)-5 and i < round(dftOfOriginal.rows / 2) +5) {
				dftMagnitude.at<int>(i, j) = 0;
			}
		}
	}
	//Affichage de la DFT avec la zone en moins
	imshow("new_dft", dftMagnitude);
	//TODO Reconvert dftManitude to dftOfOriginal ????
	//Problème au niveau de la reconversion de la dft modifiée pour pouvoir l'inverser par la suite

	//Inversion de la dft
	Mat invertedDFT;
	dft(dftOfOriginal, invertedDFT, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	//Affichage et renvoie de la DFT inversée
	imshow("Inverted DFT", invertedDFT);
	waitKey(0);
	return invertedDFT;
}


int img_processing::count_1(Mat img) {
	vector<int> counters_col(img.cols, 0);
	
	//Comptage des variations pour chaques colonnes de pixels
	for (int i = 0; i < img.cols; i++) {
		int px_prec = (int)img.at<uchar>(0, i);
		for (int j = 0; j < img.rows; j++) {

			int px = (int)img.at<uchar>(j, i);
			if (px != px_prec) {
				counters_col[i] = counters_col[i] + 1;
			}
			px_prec = px;
		}
	}

	//Renvoie du maximum de variations pour la colonne concernée
	int max = *max_element(counters_col.begin(), counters_col.end());
	int number_of_steps = round(max / 2);
	return number_of_steps;
}

int img_processing::count_2(Mat img) {
	//TODO changer le comptage, compter sur 3 ou 4 rangées de pixels au lieu d'une seule : voir pseudo code et schémas 

	//Seuil de différence entre deux moyenne pour savoir si on compte ou non
	int threshold = 5;
	//Initialisation a -1 car mean_prec est initialisée a -1, donc on aura forcément une variation 
	//car on ne peut pas avoir de moyenne négatif pour une rangée car les pixels de l'image en niveau de gris n'ont pas de valeurs négatives
	int counter = -1;
	int mean_prec = -1+threshold;//+threshold ???

	for (int j = 0; j < img.rows; j++) {
		int sum_row_i = 0;
		//Moyenne de la rangée
		for (int i = 0; i < img.cols; i++) {
			sum_row_i = sum_row_i + (int)img.at<uchar>(j, i);
		}
		int mean_row_i = sum_row_i / img.cols;
		//On ajoute si l'écart entre la moyenne et l'ancienne moyenne est supérieur au seuil
		if (mean_row_i - mean_prec>threshold and mean_row_i>255*0.039 and mean_row_i<255*0.5) {
			counter = counter + 1;
			cout << mean_row_i << endl;
		}
		mean_prec = mean_row_i;
	}

	//On division le nombre de variation par deux pour obtenir le nombre de marche car une marche est représenté par deux variations, 
	//pour la limite de la contre marche en blanc, et la marche en noir
	int number_of_steps = round(counter / 2);
	return counter;
}

