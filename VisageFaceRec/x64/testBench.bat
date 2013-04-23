REM ORIGINAL
FaceRecognizer.exe ..\data\original_20_20.txt original_20_20_EIGEN.txt -E 30
FaceRecognizer.exe ..\data\original_20_20.txt original_20_20_FISHER.txt -F 30
FaceRecognizer.exe ..\data\original_20_20.txt original_20_20_LBPH.txt -L 30
REM CROPPED
FaceRecognizer.exe ..\data\cropped_20_20.txt cropped_20_20_EIGEN.txt -E 30
FaceRecognizer.exe ..\data\cropped_20_20.txt cropped_20_20_FISHER.txt -F 30
FaceRecognizer.exe ..\data\cropped_20_20.txt cropped_20_20_LBPH.txt -L 30
REM MASKED
FaceRecognizer.exe ..\data\masked_20_20.txt masked_20_20_EIGEN.txt -E 30
FaceRecognizer.exe ..\data\masked_20_20.txt masked_20_20_FISHER.txt -F 30
FaceRecognizer.exe ..\data\masked_20_20.txt masked_20_20_LBPH.txt -L 30
PAUSE