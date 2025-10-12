# TME4 Answers

Tracer vos expériences et conclusions dans ce fichier.

Le contenu est indicatif, c'est simplement la copie rabotée d'une IA, utilisée pour tester une version de l'énoncé.
On a coupé ses réponses complètes (et souvent imprécises voire carrément fausses, deadlocks etc... en Oct 2025 les LLM ont encore beaucoup de mal sur ces questions, qui demandent du factuel et des mesures, et ont de fortes tendances à inventer).
Cependant on a laissé des indications en particulier des invocations de l'outil possibles, à adapter à votre code.

## Question 1: Baseline sequential

### Measurements (Release mode)

**Resize + pipe mode:**
```
./build/TME4 -m resize -i input_images -o output_images
... trace

./build/TME4 -m pipe -i input_images -o output_images
```



## Question 2: Steps identification

I/O-bound: 
CPU-bound: 

parallelisable a priori ?

## Question 3: BoundedBlockingQueue analysis



## Question 4: Pipe mode study

FILE_POISON ...

Order/invert :


## Question 5: Multi-thread pipe_mt

Implement pipe_mt mode with multiple worker threads.

For termination, ... poison pills...

Measurements:
- N=1: 
```
./build/TME4 -m pipe_mt -n 1 -i input_images -o output_images
...
```
- N=2: 
```
./build/TME4 -m pipe_mt -n 2 -i input_images -o output_images
...
```
- N=4: 
```
./build/TME4 -m pipe_mt -n 4 -i input_images -o output_images
...
```
- N=8: 
```
./build/TME4 -m pipe_mt -n 8 -i input_images -o output_images
...
```

Best: ??

## Question 6: TaskData struct

```cpp
struct TaskData {
... choix adopté
};
```

Fields: QImage ??? for the image data, ...

Use ??? for QImage, because ...

TASK_POISON: ...def...

## Question 7: ImageTaskQueue typing

pointers vs values

Choose BoundedBlockingQueue<TaskData???> as consequence

## Question 8: Pipeline functions

Implement reader, resizer, saver in Tasks.cpp.

mt_pipeline mode: Creates threads for each stage, with configurable numbers.

Termination: Main pushes the appropriate number of poisons after joining the previous stage.

Measurements: 
```
./build/TME4 -m mt_pipeline -i input_images -o output_images
...
```


## Question 9: Configurable parallelism

Added nbread, nbresize, nbwrite options.


Timings:
- 1/1/1 (default): 
```
./build/TME4 -m mt_pipeline -i input_images -o output_images
...
```
- 1/4/1: 
```
./build/TME4 -m mt_pipeline --nbread 1 --nbresize 4 --nbwrite 1 -i input_images -o output_images
```

- 4/1/1: 
```
./build/TME4 -m mt_pipeline --nbread 4 --nbresize 1 --nbwrite 1 -i input_images -o output_images
```
... autres configs

Best config: 
interprétation

## Question 10: Queue sizes impact


With size 1: 
```
./build/TME4 -m pipe_mt -n 2 --queue-size 1 -i input_images -o output_images
...
```

With size 100: 
```
./build/TME4 -m pipe_mt -n 2 --queue-size 100 -i input_images -o output_images
...
```

impact

Complexity: 


## Question 11: BoundedBlockingQueueBytes

Implemented with byte limit.

mesures

## Question 12: Why important

Always allow push if current_bytes == 0, ...

Fairness: ...

## Bonus

