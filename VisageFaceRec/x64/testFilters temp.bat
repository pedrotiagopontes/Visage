REM AKF
FaceRecognizer.exe ..\data\Filters\akf_20_20_A.txt akf_20_20_EIGEN.txt -E 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_B.txt akf_20_20_EIGEN_B.txt -E 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_C.txt akf_20_20_EIGEN_C.txt -E 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_D.txt akf_20_20_EIGEN_D.txt -E 30

FaceRecognizer.exe ..\data\Filters\akf_20_20_A.txt akf_20_20_FISHER.txt -F 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_B.txt akf_20_20_FISHER_B.txt -F 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_C.txt akf_20_20_FISHER_C.txt -F 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_D.txt akf_20_20_FISHER_D.txt -F 30

FaceRecognizer.exe ..\data\Filters\akf_20_20_A.txt akf_20_20_LBPH.txt -L 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_B.txt akf_20_20_LBPH_B.txt -L 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_C.txt akf_20_20_LBPH_C.txt -L 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_D.txt akf_20_20_LBPH_D.txt -L 30
PAUSE