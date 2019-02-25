int info(int age, char *units, char *name) {
    printf("i am %d %s old, and my name is %s\n", age, units, name);
}

int main(int argc, char **argv) {
    info(16, "years", "shaw");
    info(7, "days", "tach-c");
}
