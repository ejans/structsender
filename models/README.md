Template
========

This folder contains the template used to generate the structure of this project with [microblx cmake].

Usage
-----

Place the templates inside a folder into the microblx\_cmake directory and generate the template:

```bash
$ cd template
$ mkdir <microblx\_cmake path>/structsender
$ cp * <microblx\_cmake path>/structsender/
$ cd <microblx\_cmake path>
$ ./generate_pkg.lua -s structsender/structsender_package.pkg
```

[microblx cmake]: https://github.com/haianos/microblx_cmake
