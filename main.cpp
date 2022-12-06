#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <filesystem>
#include <Image.hpp>
#include <Denoiser.hpp>
#include <jsoncpp/json/json.h>

using namespace std;
int main(int argc, char **argv) {

    string mode = "denoise";

    string input_name;
    string output_name;

    if (argc == 1) {
        cout << "This program needs to be used with arguments, as follows:" << endl;
        // crop the directory name from the path
        string program_name = argv[0];
        size_t last_slash = program_name.find_last_of('/');
        program_name = program_name.substr(last_slash + 1);
        cout << "\t ./" << program_name << " [ARGUMENTS]" << endl;
        cout << "Arguments:" << endl;
        cout << "\t --mode <mode> [OPTIONAL] {'denoise', 'countour_detector', 'intensity_histogram'} (default is 'denoise')" << endl;
        cout << "\t --input <input file> [REQUIRED]" << endl;
        cout << "\t --output <output file> [OPTIONAL] (default is same as input)" << endl;
        cout << "\nTo edit the parameters used in each mode, edit the 'parameters.json' file in the root of the project." << endl;
        return 0;
    }

    if (argc % 2 == 0) {
        cout << "Error: Invalid number of arguments. Run ./main for to see the help screen" << endl;
        return 1;
    }

    bool valid = false;
    for (int i = 1; i < argc; i += 2) {
        string arg = argv[i];
        if (arg == "--mode") {
            if (strcmp(argv[i + 1], "denoise") == 0 || strcmp(argv[i + 1], "contour_detector") == 0 || strcmp(argv[i + 1], "intensity_histogram") == 0) {
                mode = argv[i + 1];
            }
            else {
                cout << "Error: Invalid mode. Run ./main for to see the help screen" << endl;
                return 1;
            }
        } else if (arg == "--input") {
            input_name = argv[i + 1];
            valid = true;
        } else if (arg == "--output") {
            output_name = argv[i + 1];
        } else {
            cout << "Error: Invalid argument: " << arg << endl;
            return 1;
        }
    }
    if (output_name.empty()) {
        output_name = input_name;
    }

    if (!valid) {
        cout << "Error: No input file specified. Run ./main for to see the help screen" << endl;
        return 1;
    }

    // fix relative path of parameters.json
    string parameters_path = "parameters.json";
    if (!filesystem::exists(parameters_path)) {
        parameters_path = "../" + parameters_path;
    }
    if (!filesystem::exists(parameters_path)) {
        cout << "Error: Could not find parameters.json file. Make sure it is in the root the project" << endl;
        return 1;
    }
    ifstream parameters_file(parameters_path);
    string parameters_string((istreambuf_iterator<char>(parameters_file)), istreambuf_iterator<char>());
    parameters_file.close();

    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(parameters_string, root);
    if (!parsingSuccessful) {
        cout << "Error: Failed to parse parameters.json" << endl;
        return 1;
    }

    Image image(input_name);

    if (mode == "denoise") {
        // check if the json file has the correct parameters
        if (!root.isMember("denoiser") || !root["denoiser"].isMember("kernel_size") || !root["denoiser"].isMember("sigma")) {
            cout << "Error: parameters.json is missing parameters for denoise mode" << endl;
            cout << "Make sure it has the following structure:" << endl;
            cout << "{" << endl;
            cout << "\t\"denoiser\":" << endl;
            cout << "\t{" << endl;
            cout << "\t\t\"kernel_size\": <int>," << endl;
            cout << "\t\t\"sigma\": <double>" << endl;
            cout << "\t}" << endl;
            cout << "..." << endl;
            cout << "}" << endl;
            return 1;
        }
        // show information about what is being done
        cout << "Denoising image: " << input_name << endl;
        cout << "Parameters used for denoising are:" << endl;
        cout << "\tKernel size: " << root["denoiser"]["kernel_size"].asInt() << endl;
        cout << "\tSigma: " << root["denoiser"]["sigma"].asDouble() << endl;
        cout << "\tOutput file: " << output_name << endl;

        Denoiser denoiser(root["denoiser"]["kernel_size"].asInt(), root["denoiser"]["sigma"].asDouble());
        Image denoised_image = denoiser.denoise(image, true);
        denoised_image.save("output", output_name);
    }

    return 0;
}