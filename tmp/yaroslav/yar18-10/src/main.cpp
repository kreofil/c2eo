/*
 * На первом этапе делается попытка разнести код по разным единицам компиляции
*/

#include "matchers.h"
#include "generator.h"
#include "util.h"


// Apply a custom category to all command-line options so that they are the
// only ones displayed.
static llvm::cl::OptionCategory MyToolCategory("c2eo options");

// CommonOptionsParser declares HelpMessage with a description of the common
// command-line options related to the compilation database and input files.
// It's nice to have this help message in all tools.
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);

// A help message for this specific tool can be added afterwards.
static cl::extrahelp MoreHelp("\nMore help text...\n");


// void generateSpace(SpaceGen &globGen, std::string objFilename,
//                    std::string initFilename) ;
void generateSpace(SpaceGen &globGen, std::string path, std::string objFilename);

std::string getShortFilename(const std::string &filename);

//--------------------------------------------------------------------------------------------------
// Главная функция, обеспечивающая начальный запуск и обход AST
int main(int argc, const char **argv) {
    std::string filename = getShortFilename(argv[1]);

    SpaceGen globGen, staticGen;
    VarGen::globalSpaceGenPtr = &globGen;
    VarGen::staticSpaceGenPtr = &staticGen;


    std::string staticObj;
    std::string staticInit;

    ApplicationGen appGen;
    std::string appCode;

    if (argc < 2) {
        llvm::errs() << "Incorrect command line format. Necessary: ./c2eo <C-file-name> --\n";
        return -1;
    }

    std::string path = "";
    if (argc > 3 && strcmp("-d", argv[argc - 2]) == 0) {
        path = argv[argc - 1];
        llvm::errs() << path;
        argc -= 2;
        path += '/';
    }

    auto ExpectedParser
            = CommonOptionsParser::create(argc, argv, MyToolCategory, llvm::cl::Optional);

    if (!ExpectedParser) {
        // Fail gracefully for unsupported options.
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    CommonOptionsParser &OptionsParser = ExpectedParser.get();
    ClangTool Tool(OptionsParser.getCompilations(),
                   OptionsParser.getSourcePathList());


////    Matchers matchers;
////    return Tool.run(matchers.getFrontEndActionFactory());
    LoopAnalyzer loopAnalyzer;
    MatchFinder finder;
    addMatchers(finder);
//     Finder.addMatcher(LoopMatcher, &loopAnalyzer);
//
    auto result = Tool.run(newFrontendActionFactory(&finder).get());

//         CodeGenerator::getCodeToConsole();

//         CodeGenerator::getCodeToFile("test.eo");
//         llvm::outs() << "code printed to file " << "test.eo" << "\n";


//     generateSpace(globGen, filename + ".glob", filename + ".glob.seq");
//     generateSpace(staticGen, filename + ".stat", filename + ".stat.seq");
    generateSpace(globGen, path + "src/", filename + ".glob");
    generateSpace(staticGen, path + "src/", filename + ".stat");


    // Тестовое формирование глобального объекта с инициализацией
    std::vector<std::string> glob;
    createGlobal(glob, path + "src/", filename);
    text2file(glob, path + "src/", "global.eo");

    std::vector<std::string> stat;
    createStatic(stat, path + "src/", filename);
    text2file(stat, path + "src/", filename+".eo");

    llvm::outs() << "\n===================================\n";
    appCode = filename;
    appGen.Generate(appCode);
    llvm::outs() << appCode;
    str2file(appCode, path, "app.eo");

    return result;
}

// Получение имени файла, как то что находится между /(если есть) и .с
std::string getShortFilename(const std::string &filename) {
    size_t st = filename.find_last_of("/");
    st =  st != std::string::npos ? st + 1 : 0;
    size_t end = filename.find_last_of(".c") - 1;
    return filename.substr(st, end - st);
}

// void generateSpace(SpaceGen &globGen, std::string objFilename,
//         std::string initFilename) {
//     std::string obj;
//     std::string init;
//     globGen.Generate(obj);
//     globGen.GenValue(init);
//     outs() << "\n===================================\n";
//     outs() << obj;
//     str2file(obj, objFilename);
//     outs() << init;
//     str2file(init, initFilename);
// }

void generateSpace(SpaceGen &globGen, std::string path, std::string objFilename) {
    std::string obj;
    std::string init;
    globGen.Generate(obj);
    globGen.GenValue(init);
    outs() << "\n===================================\n";
    outs() << obj;
    str2file(obj, path, objFilename);
    //outs() << init;
    //str2file(init, initFilename);
}
