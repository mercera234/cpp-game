#include "twod_storage_tests.h"
#include "TwoDStorage.h"
#include "TwoDStorageProxy.h"

void testSaveLoad()
{
	TwoDStorage<int> matrix;

	matrix.setDimensions(4, 7);

	matrix.fill(8);

	std::string fileName = "2dtest.2ds";
	std::ofstream os(fileName, std::ios::trunc | std::ios::binary);
	std::cout << matrix.save(os) << std::endl;
	os.close();

	TwoDStorage<int> matrix2;

	std::ifstream is(fileName, std::ios::binary);
	std::cout << matrix2.load(is) << std::endl;

	std::cout << matrix2.getDatum(20) << std::endl;
	is.close();
}

void testProxy()
{
	TwoDStorageProxy<int> proxy;
	proxy.setFileName("2dtest.2ds");

	proxy.fill(5);

	std::cout << proxy.getRows() << ' ' << proxy.getCols() << std::endl;

	
	TwoDStorageProxy<int> proxy2;
	proxy2.setFileName("2dtest.2ds");

	proxy2.setDimensions(3, 5);
	proxy2.fill(6);

	std::cout << proxy2.getSize() << ' ' << proxy2.getDatum(2, 4) << std::endl;
	system("pause");
}

void testProxyWithoutFile()
{

	TwoDStorageProxy<int> proxy;
	proxy.setFileName("2dtest.2ds");

	//proxy.setDimensions(4, 6);
	proxy.fill(6);

	std::cout << proxy.getSize() << ' ' << proxy.getDatum(2, 4) << std::endl;
	system("pause");
}
