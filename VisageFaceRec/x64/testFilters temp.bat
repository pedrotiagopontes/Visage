REM AKF
FaceRecognizer.exe ..\data\Filters\akf_20_20_A_clahe.txt akf_20_20_clahe_EIGEN.txt -E 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_B_clahe.txt akf_20_20_clahe_EIGEN_B.txt -E 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_C_clahe.txt akf_20_20_clahe_EIGEN_C.txt -E 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_D_clahe.txt akf_20_20_clahe_EIGEN_D.txt -E 30

FaceRecognizer.exe ..\data\Filters\akf_20_20_A_clahe.txt akf_20_20_clahe_FISHER.txt -F 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_B_clahe.txt akf_20_20_clahe_FISHER_B.txt -F 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_C_clahe.txt akf_20_20_clahe_FISHER_C.txt -F 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_D_clahe.txt akf_20_20_clahe_FISHER_D.txt -F 30

FaceRecognizer.exe ..\data\Filters\akf_20_20_A_clahe.txt akf_20_20_clahe_LBPH.txt -L 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_B_clahe.txt akf_20_20_clahe_LBPH_B.txt -L 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_C_clahe.txt akf_20_20_clahe_LBPH_C.txt -L 30
FaceRecognizer.exe ..\data\Filters\akf_20_20_D_clahe.txt akf_20_20_clahe_LBPH_D.txt -L 30
PAUSE