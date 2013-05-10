REM ORIGINAL
FaceRecognizer.exe ..\data\original_20_20_B.txt original_20_20_EIGEN_B.txt -E 30

FaceRecognizer.exe ..\data\original_20_20_B.txt original_20_20_FISHER_B.txt -F 30

FaceRecognizer.exe ..\data\original_20_20_B.txt original_20_20_LBPH_B.txt -L 30

REM CROPPED
FaceRecognizer.exe ..\data\cropped_20_20_B.txt cropped_20_20_EIGEN_B.txt -E 30

FaceRecognizer.exe ..\data\cropped_20_20_B.txt cropped_20_20_FISHER_B.txt -F 30

FaceRecognizer.exe ..\data\cropped_20_20_B.txt cropped_20_20_LBPH_B.txt -L 30

REM MASKED
FaceRecognizer.exe ..\data\masked_20_20_B.txt masked_20_20_EIGEN_B.txt -E 30

FaceRecognizer.exe ..\data\masked_20_20_B.txt masked_20_20_FISHER_B.txt -F 30

FaceRecognizer.exe ..\data\masked_20_20_B.txt masked_20_20_LBPH_B.txt -L 30
PAUSE