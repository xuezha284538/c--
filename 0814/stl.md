# C++ STL 算法


## 前言

STL 算法定义在头文件 `<algorithm>`，少数数值算法在 `<numeric>`。
**本质：函数模板，基于迭代器实现泛型逻辑**。
设计思想：**算法与容器解耦**，不依赖具体容器，只依赖迭代器能力；一套算法可以作用于 vector、deque、数组等任意支持对应迭代器的区间，是泛型编程思想典型落地。

> 重要前提：STL 算法区间统一为 **左闭右开 \[first, last\)**，`last` 指向区间外，禁止解引用。
> 
> 

## 一、STL 算法分类总览

1. **非修改序列算法**：只读取元素，不改动容器数据（查找、计数、遍历）

2. **修改序列算法**：复制、填充、变换、删除、替换（改变区间元素）
A
3. **排序与有序区间算法**：sort、二分查找、合并有序序列

4. **集合算法**：针对有序区间求交、并、差集

5. **堆算法**：构建堆、堆排序

6. **数值算法**：累加、内积（`<numeric>`）

> 关键知识点：
> 
> - 算法能做什么，由**迭代器类别**决定；
> 
> - `sort` 需要**随机访问迭代器** → list /forward\_list 不能使用 std::sort，要用成员 sort \(\)；
> 
> - list 的 splice、sort 是容器专属优化接口。
> 
> 

---

# 二、非修改序列算法（只读）

## 2\.1 for\_each 遍历区间

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> v = {1,2,3,4};
    // 遍历每个元素
    std::for_each(v.begin(), v.end(),
        [](int x){ std::cout << x << " "; });
    return 0;
}
```

## 2\.2 find /find\_if 线性查找

```cpp
std::vector<int> v{10,20,30,40};
auto it = std::find(v.begin(), v.end(), 30);
if(it != v.end()){
    std::cout << "找到：" << *it;
}

// find_if：条件查找
auto it2 = std::find_if(v.begin(),v.end(),
    [](int x){ return x > 20; });
```

## 2\.3 count /count\_if 计数

```cpp
std::vector<int> v{1,1,2,3,3,3};
int c1 = std::count(v.begin(), v.end(), 3);
// 统计大于1的数量
int c2 = std::count_if(v.begin(),v.end(),[](int x){return x>1;});
```

## 2\.4 adjacent\_find 查找相邻重复元素

```cpp
auto it = std::adjacent_find(v.begin(),v.end());
```

## 2\.5 min\_element /max\_element 最值

```cpp
auto maxIt = std::max_element(v.begin(),v.end());
auto minIt = std::min_element(v.begin(),v.end());
```

---

# 三、修改序列算法（会改变区间内容）

## 3\.1 copy /copy\_if 复制区间

```cpp
std::vector<int> src = {1,2,3,4};
std::vector<int> dst(4);
std::copy(src.begin(), src.end(), dst.begin());

// 只复制偶数
std::vector<int> dst2;
dst2.resize(src.size());
auto endIt = std::copy_if(src.begin(),src.end(),dst2.begin(),
    [](int x){return x%2==0;});
```

## 3\.2 fill /fill\_n 批量填充

```cpp
std::vector<int> v(5);
std::fill(v.begin(),v.end(), 0);
std::fill_n(v.begin(), 3, 99); // 前3个元素赋值99
```

## 3\.3 transform 变换映射（非常常用）

```cpp
std::vector<int> src{1,2,3};
std::vector<int> dst(src.size());
// 每个元素 *2
std::transform(src.begin(),src.end(),dst.begin(),
    [](int x){ return x*2; });
```

## 3\.4 replace /replace\_if 替换元素

```cpp
std::vector<int> v{1,2,2,3};
std::replace(v.begin(),v.end(),2,99);
// 将大于2的值替换成0
std::replace_if(v.begin(),v.end(),[](int x){return x>2;},0);
```

## 3\.5 remove /remove\_if（重点！）

> ⚠️ remove **不会真正删除内存**，只是把待删除元素移到尾部，返回新区间终点；必须搭配 erase。
> 
> 

```cpp
std::vector<int> v{1,2,3,2,4};
// 删除所有值为2的元素
auto newEnd = std::remove(v.begin(),v.end(),2);
v.erase(newEnd, v.end());

// remove_if 删除满足条件
auto newEnd2 = std::remove_if(v.begin(),v.end(),[](int x){return x%2==0;});
v.erase(newEnd2, v.end());
```

## 3\.6 reverse 区间反转

```cpp
std::reverse(v.begin(),v.end());
```

## 3\.7 unique 有序区间去重

> 前提：区间必须先排序！同样只挪动元素，配合 erase 使用。
> 
> 

```cpp
std::vector<int> v{1,1,2,2,3};
std::sort(v.begin(),v.end());
auto newEnd = std::unique(v.begin(),v.end());
v.erase(newEnd, v.end());
```

---

# 四、排序与有序区间算法

## 4\.1 sort 快速排序（混合排序： introsort）

要求：**随机访问迭代器**（vector/deque/ 数组可用；list 禁用）

```cpp
std::vector<int> v{3,1,4,2};
// 默认升序
std::sort(v.begin(),v.end());
// 降序
std::sort(v.begin(),v.end(),std::greater<int>());
// lambda自定义规则
std::sort(v.begin(),v.end(),[](int a,int b){
    return a > b;
});
```

## 4\.2 stable\_sort 稳定排序

相等元素相对顺序保持不变，性能略低于 sort。

## 4\.3 partial\_sort 局部排序（TopK 场景）

只保证前 N 个元素有序，剩余无序；适合求最大 / 最小前 K 个。

```cpp
std::vector<int> v{5,3,8,1,9,2};
// 前3个元素有序
std::partial_sort(v.begin(), v.begin()+3, v.end());
```

## 4\.4 二分查找系列（区间必须预先有序）

- `binary_search`：判断元素是否存在，返回 bool

- `lower_bound`：第一个 \>= val 的迭代器（左边界）

- `upper_bound`：第一个 \> val 的迭代器（右边界）

```cpp
std::vector<int> v{1,2,4,4,5};
bool exist = std::binary_search(v.begin(),v.end(),4);

auto left = std::lower_bound(v.begin(),v.end(),4);
auto right = std::upper_bound(v.begin(),v.end(),4);
// [left, right) 就是所有等于4的元素区间
```

> map/set 自带成员 find /lower\_bound，优先使用成员函数（效率高于全局算法）
> 
> 

## 4\.5 merge 合并两个有序区间

输出到新容器，结果依然有序

```cpp
std::vector<int> a{1,3,5}, b{2,4,6};
std::vector<int> res(a.size()+b.size());
std::merge(a.begin(),a.end(),b.begin(),b.end(),res.begin());
```

---

# 五、堆算法（底层大根堆）

头文件 `<algorithm>`，支持任意随机访问迭代器区间

```cpp
std::vector<int> v{3,1,4,2};
// 构建大根堆
std::make_heap(v.begin(),v.end());
// 堆顶元素放到末尾，重新调整堆
std::pop_heap(v.begin(),v.end());
v.pop_back();

// 新增元素
v.push_back(9);
std::push_heap(v.begin(),v.end());

// 堆排序
std::sort_heap(v.begin(),v.end());
```

> priority\_queue 默认底层就是堆结构。
> 
> 

---

# 六、集合算法（只适用于有序区间）

前提：两个区间均已升序排列

- `set_union`        并集

- `set_intersection` 交集

- `set_difference`   差集

- `set_symmetric_difference` 对称差集

```cpp
std::vector<int> a{1,2,3,4};
std::vector<int> b{3,4,5,6};
std::vector<int> res;
res.resize(a.size()+b.size());
auto endIt = std::set_intersection(a.begin(),a.end(),b.begin(),b.end(),res.begin());
res.erase(endIt, res.end());
```

# 七、数值算法 

## accumulate 累加求和

```cpp
#include <numeric>
std::vector<int> v{1,2,3,4};
int sum = std::accumulate(v.begin(),v.end(),0);
// 自定义运算：乘积
int mul = std::accumulate(v.begin(),v.end(),1,std::multiplies<int>());
```

# 八、谓词（Predicate）概念【重点】

很多算法支持传入判断规则，称为**谓词**：

1. **一元谓词**：接收 1 个参数，返回 bool（find\_if, remove\_if）

2. **二元谓词**：接收 2 个参数，返回 bool（sort 比较规则）

三种写法：

- 普通函数

- 函数对象（仿函数）

- lambda 表达式（工程最常用）

# 九、高频工程易错点汇总

1. 所有算法区间 **\[begin, end\) 左闭右开**，end 不可解引用；

2. remove/unique 仅逻辑删除，**必须配合 erase 真正释放元素**；

3. sort 需要随机访问迭代器，list、forward\_list 不能调用 std::sort；

4. binary\_search、merge、set 系列算法**要求区间预先有序**；

5. unordered 容器是无序结构，不能使用二分查找、集合算法；

6. map/set 优先使用成员函数 find/lower\_bound，性能优于全局算法；

7. 不要在遍历容器的算法内部随意增删元素，极易引发迭代器失效；

8. transform、copy 需要保证目标容器有足够空间，或使用 back\_inserter 插入迭代器：

```cpp
std::vector<int> dst;
std::copy_if(src.begin(),src.end(),std::back_inserter(dst),[](int x){return x>0;});
```

# 十、STL 算法与泛型编程关联总结

1. STL 算法全部是**函数模板**，实现一套逻辑适配多种类型；

2. 依靠迭代器隔离算法与容器，实现**类型解耦、代码复用**；

3. 和容器、迭代器三者构成 STL 完整模型，是之前模板知识点最直接的工程实践。

如果你需要，我可以额外整理一份：
**常用 STL 算法速查表 \+ 课堂随堂练习题（含代码答案）**，直接用于授课。

