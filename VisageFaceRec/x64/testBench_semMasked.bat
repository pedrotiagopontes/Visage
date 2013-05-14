REM normalized
FaceRecognizer.exe ..\data\whiteBalance\normalized_20_20_B.txt normalized_20_20_EIGEN_B.txt -E 30

FaceRecognizer.exe ..\data\whiteBalance\normalized_20_20_B.txt normalized_20_20_FISHER_B.txt -F 30

FaceRecognizer.exe ..\data\whiteBalance\normalized_20_20_B.txt normalized_20_20_LBPH_B.txt -L 30

REM equalized
FaceRecognizer.exe ..\data\whiteBalance\equalized_20_20_B.txt equalized_20_20_EIGEN_B.txt -E 30

FaceRecognizer.exe ..\data\whiteBalance\equalized_20_20_B.txt equalized_20_20_FISHER_B.txt -F 30

FaceRecognizer.exe ..\data\whiteBalance\equalized_20_20_B.txt equalized_20_20_LBPH_B.txt -L 30

REM CLAHE
FaceRecognizer.exe ..\data\whiteBalance\CLAHE_20_20_B.txt CLAHE_20_20_EIGEN_B.txt -E 30

FaceRecognizer.exe ..\data\whiteBalance\CLAHE_20_20_B.txt CLAHE_20_20_FISHER_B.txt -F 30

FaceRecognizer.exe ..\data\whiteBalance\CLAHE_20_20_B.txt CLAHE_20_20_LBPH_B.txt -L 30
PAUSE