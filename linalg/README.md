# Matmul Performance

## Reorder k + Mat A Memory Promotion
```shell
pass test.
test 1024x1024x1024:
naive exe time: Exe time = 3969.29 msec.
tile 32x32x32: Exe time = 799.312 msec.
tile 32x32x16: Exe time = 808.985 msec.
tile 32x32x8: Exe time = 977.813 msec.
tile 16x16x16: Exe time = 886.9 msec.
tile 8x8x8: Exe time = 1311.68 msec.
tile 16x16x8: Exe time = 929.061 msec.
```

## Reorder k
```shell
pass test.
test 1024x1024x1024:
naive exe time: Exe time = 2729.38 msec.
tile 32x32x32: Exe time = 326.204 msec.
tile 32x32x16: Exe time = 324.785 msec.
tile 32x32x8: Exe time = 324.538 msec.
tile 16x16x16: Exe time = 746.781 msec.
tile 8x8x8: Exe time = 1233.8 msec.
tile 16x16x8: Exe time = 479.593 msec.
```