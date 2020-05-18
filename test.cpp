#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;
// 1. 内存对齐是牺牲空间换时间 提高获取数据的效率 --> 内存读取数据是按数据块来读取的
// (1. 减少读取数据的次数 (2. 简化解析数据的过程  有些设备没有按内存对齐读的话会出现程序错误
// #pragma pack(1) // 若默认对齐数为1, 则大小为17  --> 预处理指令修改对齐数

/*


4. 析构函数: 帮助我们清理资源(不是销毁对象), 资源包括堆上的资源, 打开的文件, 网络连接
	如果对象时一个局部变量, 编译器会帮我们自动销毁
	也是编译器自动调用(在对象的生命周期结束时, 编译器自动调用析构函数 完成对资源的清理
	函数名: ~类名(){}            无参无返回值, 不能重载
	如果不定义, 编译器也会帮我们自动生成
	如果类中有自定义成员, 当前类的析构函数会去自动调用 自定义成员 的析构函数 完成自定义成员的资源清理

5. 拷贝构造 (属于构造函数的一种重载形式)
	创建一个和原有已经存在的对象完全一样的新对象 
	Data(Data date){}	传值会造成无限递归
	Data(Data& date){} 参数类型必须为引用, 不能是值(且不会通过, 引发无穷递归)
	Data(const Data& date){}
	如果不显示定义, 编译器也会自动生成一个拷贝构造(完成的是字节拷贝(也叫内容浅拷贝) 对象模型多大 就拷多大)  d = copy = copy2
	要是需要拷贝资源(深拷贝) 则需要自己定义拷贝构造



*/
class A{
public:
	int _a;// 4
	char _c;// 4 + 1 = 5
	double _d;// 5+3+ 8 = 16
	float _f;// 16 + 4 = 20
	struct B{
		int _a;// 4
		char _c;//5
		float _f;//12
	};// 12  如果没有下面那一句 则不算在A结构体大小里 大小不算他, 
	// B _b; 若有了这一句 才是32 
};// 最终大小按最大对齐数的倍数 = 8n = 24

union C{
	int _a;
	char _c;
};

void test1(){
	cout << sizeof(A) << endl; // 24
}


// 判断大小端
/*int a = 12345678
大端存放: 12 34 56 78  低地址存高位, 高地址存低位(和我们的阅读顺序一样)
小端存放: 78 56 34 12  低地址存低位, 高地址存高位 


*/
void test2(){

	// 用联合判断
	C c;
	c._c = 1;
	if (c._c == 1)
		cout << "小端" << endl;
	else 
		cout << "大端" << endl; 
	
	// 用类型强转判断
	int a = 1;
	char* pa = (char*)&a;
	if (*pa == 1)
		cout << "小端" << endl;
	else
		cout << "大端" << endl;
}

// 如何看成员相对于结构体起始位置的偏移量
// 两地址相减
void test3(){
	A  a;
	char* pa = (char*)&a;
	char* pf = (char*)&a._f;
	cout << pf - pa << endl;
}

/*
2. this 指针:  只能在"成员函数"内部使用 
类型: 类类型* const  (是一个指向不会变的参数 用struct和class定义的这些类型都是自定义类型)
始终指向当前调用函数的对象, 不同对象调同一个成员函数时, 会产生不同的行为, 
主要是因为当前指针指向的对象是不同的, 而且每一个成员函数中都有一个this指针

出现: 每个普通的成员函数中都有, 并且作为成员函数的第一个形参, 占据第一个位置
所以不同的对象调用同一个成员函数,会产生不同的结果
this 指针如果不做解引用的话, 是可以为空指针的; 否则 就会出现解引用异常

*/
class Date{
	// 每一个成员函数中都有一个this指针, 
public:
	void Display(){  // 相当于在底层的接口为 void Display(Date* const this)
		cout << _year << "-" << _month << "-" << _day << endl;
	//  cout << this->_year << "-" << this->_month << "-" << this->_day << endl;
	// 执行这些成员变量其实是通过指针去访问的, 不管有没有传this指针, 结果是一样的, 编译器可以帮我们把this指针自动传进去, 始终指向当前调用函数的对象
	//	this = &this->_month;   不支持 
	// 相当于把这个成员变量的地址赋给了this
	// 而this为const指针, 指向是不能变的, 不能赋值给他 

	}
	void SetDate(int year, int month, int day){  
	// 底层接口为: void SetDate(Date* const this, int year, int month, int day)
		_year = year;  // this->_year = year;
		_month = month;
		_day = day;
	}
	void Display2(){
		cout << "Display2()" << endl;
	}

private:
	int _year; // 年
	int _month; // 月
	int _day; // 日
};

void test4(){	
	// 不同的对象调同一个成员函数 会产生不同的结果(同为人,但并不相同) -->this指针
	// 调函数是通过this指针去访问当前函数的
	Date d1, d2; 
	d1.SetDate(2018, 5, 1);
	d2.SetDate(2018, 7, 1);
	d1.Display();  // 底层实际是这样 Display(&d1) 调用这个函数的
	d2.Display();  // Display(&d2)  然后通过this指针接收这个&d2地址, 再去访问这个对象的成员

	Date* pd = nullptr;
	pd->Display();
	pd->Display2();
	(*pd).Display();
	
}


// 类当中的6个默认成员函数 不管我们有没有定义成员函数 都会有这6个默认的成员函数
//1. 构造函数用来初始化
//2. 析构函数用来清理
//3. 拷贝函数是 使用同类对象初始化创建对象
//4. 赋值重载主要是吧一个对象赋值给另一个对象
//5/6. 取地址重载主要是普通对象和const对象取地址, 这两个很少会自己实现
/*

3. 构造函数:
作用 : 初始化对象的成员 (相当于这个空间已经有内容了, 构造函数是专门去初始化成员变量的)
且构造函数名与类名相同, 无返回值, 可以重载
如果不显示定义构造函数, 编译器会自动生成一个默认的构造函数(默认构造的函数是无参的)
若是有显示定义的构造函数, 编译器就不会再去生成一个默认构造函数, 除非我们自己定义个无参构造函数
全缺省的也算是默认构造函数, 但是默认函数只能存在一个,
	(1. 要么不写, 编译器自己生成; 2. 无参; 3. 全缺省(所有的参数都被初始化了)--> 一般操作)
如果已经显示定义了, 编译器将不再默认生成

创建对象时, 编译器根据参数的个数自动调用对应参数个数的构造函数(有参的调有参的, 无参的调无参的)
编译器自动调用构造函数来初始化, 我们自己不能调用, 不用再调设置函数

无参构造后面不能加();
Data a;  前提必须自己显示定义一个无参构造函数,
Data a();  相当于是一个函数的声明 没有实际的指令

如果类中有自定义成员, 当前类的构造函数会去自动调用 自定义成员 的默认构造函数 完成自定义成员的初始化

//构造函数： 初始化成员
//函数名与类型相同， 无返回值， 可以重载
//如果不显示定义，编译器会自动生成一个默认的构造函数
//如果已经显示定义了构造函数，编译器奖不在默认生成
//创建对象时，编译器自动调用对应的构造函数
//默认构造： 编译器自动生成，无参，全缺省，但是只用存在一个
//全缺省，也是默认构造函数
*/

class Date{
public:
	// 下面这些就是构造函数 函数名和类名是相同的 且没有返回值 还可以重载 --> 构造函数是有多个的
	// 不需要我们亲自去调用, 创建对象的时候编译器亲自去调用
	Date(int year, int month, int day){
		this->_year = year;
		_month = month;
		_day = day;
	}

	Date(int year = 1, int month = 1, int day = 1){
	//Date(int year, int month, int day){
		->_year = year;
		_month = month;
		_day = day;
		cout << "Date(int year = 1, int month = 1, int day = 1)" << endl;
	}

	// 这是构造函数的重载 这个供有一个参数的对象调用
	Date(int year){
		_year = year;
	}
	
	//重载的构造函数, 无参的为默认构造 只供无参对象调用
	Date(){
		cout << "Date()" << endl;
	}

private:
	int _year;
	int _month;
	int _day;

};

void test5(){
	// 如果我们不显示定义, 编辑器会默认生成无参构造函数
	//Date d1, d2;  // 这行代码相当于是类的实例化 这两个对象的内容是空的 这里没有显式定义, 但是也调用了无参构造  这里的d1, d2为无参的对象
	//d1.Display(); // 没有设置之前对象的内容是随机值
	//d2.Display();

	//d1.SetDate(2018, 5, 1);  // 这里才是对成员变量的初始化
	//d2.SetDate(2018, 7, 1);
	
	// 相比上面的初始化 构造函数就更简单的了, 
	Date d1(2020, 5, 8); // 在运行到这里(实例化对象)时 编译器会自动调用构造函数
	Date d2(2020, 5, 10);  // 这下就不用再去写设置函数(setData)了
	// 同时我们并不能显式的去调用这个构造函数 
	// d1.Date(2020, 5, 10);    是错的
	Date d3; // 这里会调无参构造, 若我们没有显示定义构造函数 则可以成功创建无参对象, 因为编译器会默认定义一个无参构造函数; 若我们显示定义了构造函数,编译器就不会再定义无参构造了, 而则这里就创建无参对象就会失败了,  因为没有无参构造函数供他调用; 若此时还需要创建无参对象 则就需要再次显示定义无参构造函数
	Date d3(); // 这个不是无参构造, 相当于是声明了一个函数. 这个函数没有参数 返回值为Date 并没有实际指令 和调用


}


/*
当构造函数的参数是一个对象时就是拷贝构造
//拷贝构造: 创建一个和已经存在的对象完全一样的新对象
//参数类型为引用，不能是值，否则会引发无穷递归
//也属于构造函数的一种重载形式
//如果不显示定义，编译器会自动生成一个拷贝构造, 完成字节拷贝，即浅拷贝

Date(const Date& date){
	_year = date._year;
	_month = date._month;
	_day = date._day;
	cout << "Date(const Date& date)" << endl;
}


//析构函数： 资源清理  ---> 堆上的资源，打开的文件，网络连接等
//对象的生命周期结束时，编译器自动调用
//函数名： ~类名，  无参无返回值，不能重载
//如果不定义，编译器会自动生成
~Date(){
cout << "~Date()" << endl;
}

*/

class E{
public:

	//如果有自定义成员，当前类的构造函数会自动调用自定义成员的默认构造函数，完成自定义成员的初始化
	E(){

	}
	E(int a){

	}
	//析构函数会自动调用自定义成员的析构函数，完成自定义成员的资源清理
	~E(){
	}
private:
	Date _d;
};


void test5(){
	Date d1(2020, 5, 8);  // 编译器自动调用构造函数
	Date d2(2020, 5, 10);  // 编译器自动调用构造函数
	//Date d3;  //调用无参构造
	//Date d4(); //声明一个函数
	//Date d4(2020);
	d1.Display();  // Display(&d1)
	d2.Display();  // Display(&d2)

	d1.SetDate(2018, 5, 1);
	d2.SetDate(2018, 7, 1);
	d1.Display();  // Display(&d1)
	d2.Display();  // Display(&d2)

}

void test6(){
	E e;  //编译器会自动生成一个无参的默认构造
	E e2(2);
}

void test7(){
	Date d;
}


void test8(){
	Date d(2020, 5, 5);
	Date copy(d);  //拷贝构造
	Date copy2 = d;  //拷贝构造
}

class seqList{
public:
	seqList(int n = 10){
		_array = (int*)malloc(sizeof(int)* n);
		_size = 0;
		_capacity = n;
	}
	//需要自己实现拷贝构造，完成资源拷贝
	~seqList(){
		if (_array)
			free(_array);
		_size = 0;
		_capacity = 0;
		_array = nullptr;
		cout << "~seqList()" << endl;
	}
private:
	int* _array;
	int _size;
	int _capacity;
};

void test9(){
	seqList lst;
	seqList copy(lst);  //拷贝构造
}

int main(){
	//test();
	//test2();
	//test3();
	//test4();
	//test5();
	//test6();
	//test7();
	//test8();
	test9();

	system("pause");
	return 0;
}
