REM bilateral
FaceRecognizer.exe ..\data\Filters\bilateral_20_20.txt bilateral_20_20_EIGEN.txt -E 30
FaceRecognizer.exe ..\data\Filters\bilateral_20_20.txt bilateral_20_20_FISHER.txt -F 30
FaceRecognizer.exe ..\data\Filters\bilateral_20_20.txt bilateral_20_20_LBPH.txt -L 30
REM gaussian
FaceRecognizer.exe ..\data\Filters\gaussian_20_20.txt gaussian_20_20_EIGEN.txt -E 30
FaceRecognizer.exe ..\data\Filters\gaussian_20_20.txt gaussian_20_20_FISHER.txt -F 30
FaceRecognizer.exe ..\data\Filters\gaussian_20_20.txt gaussian_20_20_LBPH.txt -L 30
PAUSE