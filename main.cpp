#include <unordered_map>
#include <functional> // Для std::hash
#include <tuple>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <iterator>

using namespace std;

template <typename T = int, size_t N >
std::array<T,N> get_array_value_from_str(std::string line) {
  std::istringstream iss{ line };
  std::array<T,N> val{};
  std::copy(std::istream_iterator<T>(iss), std::istream_iterator<T>(), val.begin());
  return val;
}

using T_map = std::unordered_map<std::string, int, std::hash<std::string>>;

template <typename T, std::size_t N, typename U >
auto array_to_tuple(const std::array<T, N>& arr,U val) {
  return std::apply(
      [& val](auto&&... args) { return std::make_tuple(std::forward<decltype(args)>(args)...,val); },
      arr);
}


template<typename Matrix_>
struct Tdept;

template<typename T,T value, std::size_t dimen = 2>
struct Matrix: public T_map{
    std::string key;
    std::vector<int> dept_;
public:
    class TIterator : public T_map::iterator {
    public:
        using BaseIterator = typename T_map::iterator;
        TIterator(BaseIterator base) : BaseIterator(base) {}
         auto operator*() const {
             auto [key,v] = BaseIterator::operator*();
             return array_to_tuple (get_array_value_from_str<T,dimen>(key),v);
         }
     };

    using Tvalue_matrix = T;

    TIterator begin() { return TIterator (T_map::begin());}
    TIterator end() {return TIterator (T_map::end());}
    std::size_t get_dimen(){
        return dimen;
    }
    auto  operator [](int index);
    T get_value(){
        try{
            return T_map::at(key);
        } catch (const std::out_of_range& e) {
            return value;
        }
        return value;
    }
    auto set_value(T v){
        T_map::operator[](key) = v;
        return *this;
    }
};

template<typename Matrix_>
struct Tdept{
    Matrix_ &mtrx;
    Tdept(Matrix_ &mtrx):mtrx(mtrx){}
    auto operator [](int index){
        if(mtrx.dept_.size()==mtrx.get_dimen()){
            throw " Error dept_ is wrong";
        }
        mtrx.key+= std::to_string(index)+" ";
        mtrx.dept_.push_back(index);
        return Tdept(mtrx);
    }
    template<typename T = typename Matrix_::Tvalue_matrix>
    operator T() const{
        return static_cast<T>(mtrx.get_value());
    }

	Tdept& operator=(typename Matrix_::Tvalue_matrix value) {
        mtrx.set_value(value);
        return *this;
    }
	friend std::ostream& operator<<(std::ostream& out, const Tdept<Matrix_>& matrix) {
		out << matrix.mtrx.get_value();
		return out;
	}
};


template<typename T,T value, std::size_t dimen>
auto Matrix<T,value,dimen>::operator [](int index){
        dept_.clear();
        key.clear();
        return Tdept<Matrix>(*this)[index];
}

int main()
{
    Matrix<int,-1,2> d{};
    for(int i =0;i<10;i++){
       d[i][i]=i;
    }
    for(int i =9;i>=0;i--){
       d[i][9-i]=i;
    }

    for(int i=1;i<9;i++){
        for(int j=1;j<9;j++){
            cout << d[i][j] <<" ";
        }
        cout <<"\n";
    }

    ((d[100][100]=314)=0)=217;

    cout <<"d[100][100] = " << d[100][100]<<"\n";

    cout << "d.size(): "<<d.size()<<"\n";

    cout <<"d:\n";
    for(const auto[x,y,v]: d){
        cout << x << y << v << std::endl;
    }


    Matrix<int,-1,3> d3{};

    for(int i =0;i<10;i++){
       d3[i][i][i]=i;
    }

    cout <<"d3:\n";
    for(const auto[x,y,z,v]: d3){
        cout << "x:"<< x <<" y:"<< y  <<" z:"<< z <<" value: "<< v << std::endl;
    }
    return 0;
}
