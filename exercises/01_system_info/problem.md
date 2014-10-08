## System Info

To get started with libuv we'll just call some methods it provides that give us info about the system. **WHAT no event loop?**
Yeah not yet - that'll come in the next exercise, we wanna keep it simple for now ;)

The program, that you'll find inside `src/01_system_info.c` currently gets system uptime information, here it is:

```cpp
int err;

double uptime;
err = uv_uptime(&uptime);
CHECK(err, "uv_uptime");
log_info("Uptime: %f", uptime);
log_report("Uptime: %f", uptime);
```

As you can see most `uv_*` methods return an `err` object (similar to Node.js). Make sure to always `CHECK` that value
in order to catch those errors early. 

Since only one value can be returned `uv_uptime` takes a [pointer][] to the address of `uptime` in order to store the result there.
The main important part is calling `log_report` with that result since that is what **learnuv** uses to verify your
solution. You can use `log_info` as much as you want in order to print helpful output to the terminal, but it won't affect
your report.

Your task is to obtain the **RSS** (resident set size) of your system via the `uv_resident_set_memory` in a similar
fashion as we did for `uv_uptime`. Once you obtained the value report it as follows:

```cpp
log_report("RSS: %ld", resident_set_memory);
```

After you successfully ran your program run `./learnuv verify` in order to verify your result. Do the same for all
upcoming exercises as well.

pointer:http://www.cprogramming.com/tutorial/c/lesson6.html
