# choiceMaker.h

choiceMaker.h is a lightweight C library to navigate and select items directly from the terminal.

### Install

Simply put `choiceMaker.h` in your `includes` folder.
*Yeah it's simple like that*

You can download with that : 
```sh
curl -O https://github.com/wirenux/choiceMaker.h/blob/main/choiceMaker.h
``` 
or
```sh
wget https://github.com/wirenux/choiceMaker.h/blob/main/choiceMaker.h
```
### How to use ?

To use `choiceMaker.h` in a **C** file you need to adds:

```c
#include "../includes/choiceMaker.h" // Adjust path if necessary
```

Then you can call `choice_from_array()` to launch the menu:
```c
const char *options[] = {"Option 1", "Option 2", "Option 3"};
const char *choice = choice_from_array("Select an option:", options, 3);

printf("You selected: %s\n", choice);
```

### Example
```c
#include <stdio.h>
#include "choiceMaker.h"

int main(void) {
    const char *options[] = {"Apple", "Banana", "Cherry"};
    const char *choice = choice_from_array("Select a fruit:", options, 3);

    printf("You selected: %s\n", choice);
    return 0;
}
```

#### Notes
- Works in terminal using arrow keys to navigate and Enter to select.
- Only supports Unix-like systems (Linux/macOS) because of `termios` and `unistd.h`.


### Dependencies

- **Standard C** – `stdio.h`, `stdlib.h`, `string.h`
- **POSIX / Terminal** – `unistd.h`, `termios.h`

> Anyone is free to copy, modify, publish, use, compile, sell, or distribute this software, either in source code form or as a compiled binary, for any purpose, commercial or non-commercial, and by any means.
