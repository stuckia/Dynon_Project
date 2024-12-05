# Dynon_Project

To run:
```
gcc -o IPC_Project IPC_Project.c

./IPC_Project
```

I am aware I shared the link to this on 12/4, but it has occurred to me on 12/5 that (although grabbing a string from the "Even"/"Odd" was not required and I could have returned any generic string) the more efficient way to have done:

    33 | strcpy(message.msg_data.string_value, ((i % 2 == 0) ? "Even" : "Odd"));

Should have instead been:

    33 | strcpy(message.msg_data.string_value, ((i & 1) ? "Odd" : "Even"));

I recognize that saving time is vital in a larger program and that multiplication/division/modulus would take longer than a bitwise operation. With 10 iterations this is not particularly notable but if it were to run with, say, 10000 the time would make a difference.

Again, thank you so much for your time and consideration!
