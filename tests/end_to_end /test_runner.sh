#!/bin/bash

mkdir -p results

for file in *.in ; do 
    echo "Обработка файла $file"

    echo "Исполнение std::set:"
    ../../build/standard_set/set     < "$file" > "results/std_set_${file%.out}.out"
    
    echo "Исполнение AVL tree:"
    ../../build/my_avl_tree/avl_tree < "$file" > "results/avl_tree_${file%.out}.out"
    
    echo "Сравнение результатов:"
    diff "results/std_set_${file%.out}.out" "results/avl_tree_${file%.out}.out" > /dev/null
    if [[ $? -eq 0 ]]; then
        echo "Ответы одинаковые"
    else
        echo "Ответы разные :("
    fi
    
    echo "==================================="
done