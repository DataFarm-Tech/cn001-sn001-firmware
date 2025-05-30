// src/add_numbers.S
.global add_two_numbers
.type add_two_numbers, @function

add_two_numbers:
    add a2, a2, a3  // a2 = a2 + a3
    ret
