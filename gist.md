1. https://www.geeksforgeeks.org/readwrite-class-objects-fromto-file-c/

2. https://stackoverflow.com/questions/18015069/fwrite-and-fread-for-vector-return-segmentation-fault
With C++ there are simpler ways of doing this, if you learn to use the C++ I/O stream library and some nice standard algorithms and use iterators.

To write a vector to a file:

std::ofstream os{"ciphertext", std::ios::out};

std::copy(std::begin(c), std::end(c),
          std::ostream_iterator<long long int>(os));
And to read from the file:

std::ifstream is{"ciphertext", std::ios::in};

std::copy(std::istream_iterator<long long int>(is),
          std::istream_iterator<long long int>(),
          std::back_inserter(c));
There's actually an even simpler way to read from a file into a vector:

std::ifstream is{"ciphertext", std::ios::in};

std::vector<long long int> c(std::istream_iterator<long long int>(is),
                             std::istream_iterator<long long int>());
This relies on the std::vector constructor taking two iterators as arguments.

If you don't want to use text files, but binary files, you unfortunately have to loop manually and write/read the data, i.e. you manually have to do what std::copy does for you.

Something like this to write the data:

std::ofstream os{"ciphertext", std::ios::out | std::ios::binary};

for (const auto& value : c)
    os.write(reinterpret_cast<const char*>(&value), sizeof(value));
And like this to read it:

std::ifstream is{"ciphertext", std::ios::in | std::ios::binary};

long long int value:
while (is.read(reinterpret_cast<char*>(&value), sizeof(value)))
    c.push_back(value);
If you don't have the C++11 range-based for loop (used in the writing example above), use a normal classic iteration for loop:

std::vector<long long int>::const_iterator i;
for (i = c.begin(); i != c.end(); ++i)
    os.write(reinterpret_cast<const char*>(&(*i)), sizeof(*i));
