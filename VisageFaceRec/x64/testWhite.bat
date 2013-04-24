REM norm_20_20
FaceRecognizer.exe ..\data\whiteBalance\normalized_20_20.txt norm_20_20_EIGEN.txt -E 30
FaceRecognizer.exe ..\data\whiteBalance\normalized_20_20.txt norm_20_20_FISHER.txt -F 30
FaceRecognizer.exe ..\data\whiteBalance\normalized_20_20.txt norm_20_20_LBPH.txt -L 30
REM equalized_20_20
FaceRecognizer.exe ..\data\whiteBalance\equalized_20_20.txt equalized_20_20_EIGEN.txt -E 30
FaceRecognizer.exe ..\data\whiteBalance\equalized_20_20.txt equalized_20_20_FISHER.txt -F 30
FaceRecognizer.exe ..\data\whiteBalance\equalized_20_20.txt equalized_20_20_LBPH.txt -L 30
REM CLAHE_20_20
FaceRecognizer.exe ..\data\whiteBalance\CLAHE_20_20.txt CLAHE_20_20_EIGEN.txt -E 30
FaceRecognizer.exe ..\data\whiteBalance\CLAHE_20_20.txt CLAHE_20_20_FISHER.txt -F 30
FaceRecognizer.exe ..\data\whiteBalance\CLAHE_20_20.txt CLAHE_20_20_LBPH.txt -L 30
PAUSE