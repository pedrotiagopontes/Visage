#include "utils.h"
#include "algorithmsTest.h"

int main(int argc, const char *argv[]) {
    //string att_path = "..\\etc\\at.txt";
    //string lfw_color_path = "..\\etc\\lfw_color.txt";
	string path = "..\\etc\\at.txt";
	string outputfilename = "..\\results\\default.txt";

	if (argc > 1) {
        path = string(argv[1]);
    }

    if (argc == 3) {
		outputfilename = string(argv[2]);
    }

	algorithmsTest(path, outputfilename);

	//waitKey(0);
	return 0;
}
