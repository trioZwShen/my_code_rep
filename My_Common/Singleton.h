/**
 * @file 	: Singleton.h
 * @author	: neilzwshen
 * @time	: 2018-7-30
 * @remark	: 单例模板, 只要将对象作为T, 就可以获取到一个单例对象, 构造函数不能传参
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

template<class T>
class Singleton {
public:
	/**
	 * 单例获取
	 * @return [返回T的单例对象]
	 */
	static T* GetInst(){
		if (!flag_instance){
			flag_instance = new Singleton();
		}
		return &flag_instance->_instance;
	}

protected:
	/**
	 * 单例构造
	 */
	Singleton():_instance(){}

private:
	/**
	 * T对象实例
	 */
	T _instance;
	/**
	 * 单例模板实例,
	 */
	static Singleton<T> * flag_instance;
};

template<class T>
Singleton<T> * Singleton<T>::flag_instance = 0;


#endif