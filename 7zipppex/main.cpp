#include <tchar.h>
#include <7zpp/7zpp.h>

//압축 파일의 생성
int CreateArchive(int argc, TCHAR** argv)
{
    //생성할 압축 파일의 이름
    const TCHAR* archiveName = argv[2];
    //압축 대상이 될 폴더
    const TCHAR* targetDir = argv[3];

    //라이브러리 초기화
    SevenZip::SevenZipLibrary lib;
    lib.Load();
    //압축을 수행한다.
    SevenZip::SevenZipCompressor compressor(lib, archiveName);
    compressor.CompressDirectory(targetDir, nullptr);
    return 0;
}
//압축 파일의 해제
int ExtractArchive(int argc, TCHAR** argv)
{
    //압축 파일의 이름
    const TCHAR* archiveName = argv[2];
    //압축 파일이 해제될 경로
    const TCHAR* destination = argv[3];
    //라이브러리의 초기화
    SevenZip::SevenZipLibrary lib;
    lib.Load();
    //압축 파일의 내용을 특정 폴더에 해제한다.
    SevenZip::SevenZipExtractor extractor(lib, archiveName);
    extractor.ExtractArchive(destination, nullptr);
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    if (argc != 4)
    {
        printf("usage : 7zipppex a test .");
        return 0;
    }

    if(argv[1][0] == 'a')
        CreateArchive(argc, argv);

    return 0;
}