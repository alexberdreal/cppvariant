#include "../variant.h"

int main() {

    {
        // Test the default constructor
        cppvariant::Variant<int, char, size_t> data;

        // Assignment operator
        data = int(4);
        // Value getter
        assert(*data.Get<int>() == 4);

        // Assignment operator
        data = size_t(5);
        // Value getter
        assert(*data.Get<size_t>() == 5);

        // Assignment operator
        data = char('a');
        // Value getter
        assert(*data.Get<char>() == 'a');

        // Wrong type value getter
        assert(data.Get<int>() == nullptr);
        // Wrong type value getter
        assert(data.Get<size_t>() == nullptr);
    }

    {
        // Test the user-defined constructor
        cppvariant::Variant<int, char, size_t> data { 7ul };

        // Value getter
        assert(*data.Get<size_t>() == 7ul);

        // Assignment operator
        data = char('b');
        // Value getter
        assert(*data.Get<char>() == 'b');
    }

    return 0;
}