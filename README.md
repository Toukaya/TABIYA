# Tabiya
Tabiya is a modern C++ library that provides elegant abstractions for range-based iteration. Built with C++20 concepts, it offers a flexible and intuitive way to work with iterators, ranges, and repeated operations.
## Features
- **Concept-based Design**: Utilizes C++20 concepts for compile-time behavior enforcement
- **Iterator Wrapper**: Flexible wrapper for any iterator-like type
- **Range Creation**: Easy creation of integer or custom type ranges with forward and reverse iteration
- **Times Iterator**: Simplified counting loops with automatic iteration
- **STL Compatibility**: Works seamlessly with standard library algorithms
- **Customizable Behavior**: Extensible design with support for custom incrementors, decrementors, and dereferencers

## Requirements
- C++20 compatible compiler (tested with Clang, GCC, and MSVC)
- CMake 3.12 or higher (for building)
- Catch2 (for tests)

## Usage Examples
### Iterator Wrapper
The `IterWrapper` class provides a flexible wrapper around any iterator-like type:
``` cpp
#include <tabiya/IterWrapper.hpp>
#include <vector>
#include <numeric>

std::vector<int> numbers = {1, 2, 3, 4, 5};
tabiya::IterWrapper begin(numbers.begin());
tabiya::IterWrapper end(numbers.end());

// Use with standard algorithms
int sum = std::accumulate(begin, end, 0);  // 15

// Manual iteration
for (auto it = begin; it != end; ++it) {
    std::cout << *it << " ";  // 1 2 3 4 5
}
```
### Range
The `Range` class provides a way to iterate over a sequence of values:
``` cpp
#include <tabiya/Range.hpp>
#include <iostream>

// Forward range from 1 to 5 (exclusive)
for (auto value : tabiya::Range<int>{}.from(1).to(5)) {
    std::cout << value << " ";  // 1 2 3 4
}

std::cout << std::endl;

// Reverse range from 10 to 5 (exclusive)
for (auto value : tabiya::Range<int>{}.from(10).to(5)) {
    std::cout << value << " ";  // 10 9 8 7 6
}
```
### Times
The `Times` class provides a convenient way to repeat an operation a specific number of times: 
``` cpp
#include <tabiya/Times.hpp>
#include <iostream>

// Iterate 5 times
for (auto i : tabiya::Times<int>(5)) {
    std::cout << "Iteration " << i << std::endl;
    // Iteration 0
    // Iteration 1
    // Iteration 2
    // Iteration 3
    // Iteration 4
}
```
### Custom Behavior
You can customize the behavior of all Tabiya components:
``` cpp
#include <tabiya/Range.hpp>
#include <iostream>

// Custom incrementor that advances by 2
struct CustomIncrementor {
    void operator()(int& value) const { value += 2; }
};

// Custom dereferencer that doubles the value
struct DoubleValueDereferencer {
    auto operator()(int& value) const -> int { return value * 2; }
};

// Range with custom incrementor
tabiya::Range<int, CustomIncrementor> evenRange;
evenRange.from(1).to(10);
for (auto value : evenRange) {
    std::cout << value << " ";  // 1 3 5 7 9
}

std::cout << std::endl;

// Range with custom dereferencer
tabiya::Range<int, tabiya::DefaultIncrementor<int>, DoubleValueDereferencer> doubledRange;
doubledRange.from(1).to(5);
for (auto value : doubledRange) {
    std::cout << value << " ";  // 2 4
}
```
## Core Components
### 1. Concepts
The library defines a rich set of concepts that are used throughout the codebase:
- `Addable`,`Subtractable`,`AddSubtractable`: Arithmetic operation concepts 
- `EqualityComparable`,`InequalityComparable`, etc.: Comparison concepts 
- `Dereferenceable`,`PrefixIncrementable`, etc.: Iterator operation concepts 
- `Numeric`,`IsSigned`, etc.: Numeric type concepts

### 2. IterWrapper
A flexible iterator wrapper that can adapt any iterator-like type with customizable behavior:
- **Incrementor**: Controls how the iterator advances 
- **Dereferencer**: Controls how values are accessed 
- **EqualityComparator**: Controls how iterators are compared 

### 3. Range
A range generator that produces a sequence of values from a start to an end point:
- Support for both ascending and descending ranges
- Works with any type that satisfies the necessary concepts
- Compatible with STL algorithms and containers

### 4. Times
A utility for creating iteration loops a specific number of times:
- More expressive than traditional counting loops
- Works with various numeric types
- Handles edge cases like negative counts gracefully

## Advanced Usage
### Integration with STL Algorithms
All Tabiya components work seamlessly with STL algorithms:
``` cpp
#include <tabiya/Range.hpp>
#include <algorithm>
#include <vector>

tabiya::Range<int> range;
range.from(1).to(10);

// Find the first even number
auto it = std::find_if(range.begin(), range.end(), [](int n) { return n % 2 == 0; });
// *it == 2

// Calculate sum of all numbers in range
int sum = std::accumulate(range.begin(), range.end(), 0);
// sum == 45 (1+2+3+4+5+6+7+8+9)

// Create a vector from a range(Not recommended)
std::vector<int> vec(range.begin(), range.end());
// vec == {1, 2, 3, 4, 5, 6, 7, 8, 9}
```
### Creating Custom Sequence Generators
You can create your own sequence generators by combining Tabiya components:
``` cpp
#include <tabiya/Range.hpp>
#include <cmath>

// Custom dereferencer that generates squares
struct SquareDereferencer {
    auto operator()(int& value) const -> int {
        return value * value;
    }
};

// Use with Range to generate squares
tabiya::Range<int, tabiya::DefaultIncrementor<int>, SquareDereferencer> squaresRange;
squaresRange.from(1).to(6);

for (auto value : squaresRange) {
    std::cout << value << " ";  // 1 4
}
```
## License
[Apache License](LICENSE)
