#include <iostream>


namespace MY_TUPLE
{
    //可变参数模版必须有主模版   ...代表此模版参数是个包
    template <typename ... __args_type>
    class tuple;

    //参数中什么都没有的情况
    template <>
    class tuple <>
    {
    public:
        tuple() {}
    };
    
    //参数有很多的情况，将参数分为this_type 和 args_type两个部分, 类模版必须显示的说明参数类型
    template <typename __this_type, typename ... __args_type>
    //使用类模版的时候，必须在<>中显示声明类模版参数，...代表此类型参数是一个包
    class tuple <__this_type, __args_type ...> : public tuple<__args_type ...>
    {
    public:
        tuple(__this_type val, __args_type ... params) : tuple<__args_type ...>(params ...)
        {
            value_ = val;
        }
 
        __this_type get_value()
        {
            return value_;
        }

    public:
        __this_type value_;
    };


    //可变参数模版必须有主模版
    template <int N, typename ... __args_type>
    struct element;


    //模版特化的结果，即，如果tuple中没有元素就使用下面这个模版
    template <int N>
    //同样，使用类模版的时候要显示声明模版参数
    struct element<N, MY_TUPLE::tuple<>> {//就是说当tuple里面是空着的话，如果N大于0，那么就出错
        //std::cout<<N<<std::endl;
        static_assert(0 > N, "Index outside of tuple!");//如果第一条是false，那么就会错误
    };

    //正常的模版，并且使用类模版的时候要显示声明模版参数
    template <int N, typename __this_type, typename ... __args_type>
    struct element<N, MY_TUPLE::tuple<__this_type, __args_type ...>> : public element<N - 1, MY_TUPLE::tuple<__args_type ...>> {};


    //下面这个是模版特化的结果，即，如果N=0就使用下面这个模版
    //并且使用类模版的时候要显示声明模版参数
    template <typename __this_type, typename ... __args_type>
    struct element<0, MY_TUPLE::tuple<__this_type, __args_type ...>>
    {
        using value_type = __this_type;
        using class_type = MY_TUPLE::tuple<__this_type, __args_type ...>;
    };

    //函数模版，从调用此函数的实参来推断函数模版中的参数
    template <int N, typename ... __args_type>
    auto get(MY_TUPLE::tuple<__args_type ...> & tu)
    {
        using __class_type = typename element<N, MY_TUPLE::tuple<__args_type ...>>::class_type;//调用element必须在<>中指明模版参数类型
        __class_type* p;//获取tuple中第一个元素为想要了解的元素时的tuple类型
        p = &tu;//将获取的类型绑定到整个tuple中，使用到了”类的父类的指针可以绑定到子类的对象上“这一特性
        return p->get_value();//通过父类的指针访问对应的值
    }
}


int main(int argc, const char * argv[]) {
    MY_TUPLE::tuple<int, char, double> tu_(1, 'A', 3.1415926);
    std::cout << MY_TUPLE::get<0>(tu_) << " " << MY_TUPLE::get<1>(tu_) << " " << MY_TUPLE::get<2>(tu_)<< MY_TUPLE::get<2>(tu_)<<std::endl;
    return 0;
}
