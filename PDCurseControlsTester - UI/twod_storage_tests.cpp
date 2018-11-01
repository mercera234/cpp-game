#include "twod_storage_tests.h"
#include "TwoDStorage.h"
#include "TwoDStorageProxy.h"
#include "FilePath.h"

void testSaveLoad()
{
	TwoDStorage<int> matrix;

	matrix.setDimensions(4, 7);

	matrix.fill(8);

	std::string fileName = "2dtest.2ds";

	{
		std::ofstream os(fileName, std::ios::trunc | std::ios::binary);
		matrix.save(os);
	}

	TwoDStorage<int> matrix2;

	std::ifstream is(fileName, std::ios::binary);
	matrix2.load(is);

	std::cout << matrix2.getDatum(20) << std::endl;
	system("pause");
}

void hardDriveTest()
{
	TwoDStorage<int> matrix;
	matrix.setDimensions(9, 6);
	matrix.fill(36);

	FilePath hd;
	hd.setObject(&matrix);

	std::cout << hd.save("hdSave.2ds") << std::endl;
	system("pause");
}

void testProxy()
{
	TwoDStorageProxy<int> proxy;
	FilePath fs;
	fs.setPath("data");
	fs.setFileName("2dtest.2ds");
	proxy.setFilePath(&fs);

	proxy.fill(5);

	std::cout << proxy.getRows() << ' ' << proxy.getCols() << ' ' << proxy.getDatum(0) << std::endl;

	
	TwoDStorageProxy<int> proxy2;
	proxy2.setFilePath(&fs);

	proxy2.setDimensions(3, 5);
	proxy2.fill(6);

	std::cout << proxy2.getSize() << ' ' << proxy2.getDatum(2, 4) << std::endl;
	system("pause");
}

void testProxyWithoutFile()
{
	TwoDStorageProxy<int> proxy;

	proxy.setDimensions(4, 6);
	proxy.fill(6);

	std::cout << proxy.getSize() << ' ' << proxy.getDatum(2, 4) << std::endl;
	system("pause");
}

void testProxyWithFileSystem()
{
	TwoDStorageProxy<int> proxy;
	FilePath fs;
	fs.setPath("data");
	fs.setFileName("2dtest.2ds");
	//proxy.setFilePath(&fs);
	fs.setObject(&proxy);

	int bytesLoaded = fs.load();

	std::cout << proxy.getSize() << ' ' << proxy.getDatum(2, 4) << ' ' << bytesLoaded << std::endl;
	system("pause");
}