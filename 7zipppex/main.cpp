#include <tchar.h>
#include <7zpp/7zpp.h>

//���� ������ ����
int CreateArchive(int argc, TCHAR** argv)
{
    //������ ���� ������ �̸�
    const TCHAR* archiveName = argv[2];
    //���� ����� �� ����
    const TCHAR* targetDir = argv[3];

    //���̺귯�� �ʱ�ȭ
    SevenZip::SevenZipLibrary lib;
    lib.Load();
    //������ �����Ѵ�.
    SevenZip::SevenZipCompressor compressor(lib, archiveName);
    compressor.CompressDirectory(targetDir, nullptr);
    return 0;
}
//���� ������ ����
int ExtractArchive(int argc, TCHAR** argv)
{
    //���� ������ �̸�
    const TCHAR* archiveName = argv[2];
    //���� ������ ������ ���
    const TCHAR* destination = argv[3];
    //���̺귯���� �ʱ�ȭ
    SevenZip::SevenZipLibrary lib;
    lib.Load();
    //���� ������ ������ Ư�� ������ �����Ѵ�.
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