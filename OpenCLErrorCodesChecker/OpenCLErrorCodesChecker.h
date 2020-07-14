
#ifndef MAICRA_OPENCLERRORCODESCHECKER_H
#define MAICRA_OPENCLERRORCODESCHECKER_H


#include <string>
#include "glad/glad.h"

class OpenCLErrorCodesChecker {
public:
    std::string getErrorString(int error);
};


#endif //MAICRA_OPENCLERRORCODESCHECKER_H
