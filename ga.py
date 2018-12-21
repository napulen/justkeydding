from optimizer import generator, evaluator, evolver
import logging

if __name__ == '__main__':
    population_size = 5
    maximum_range = 100
    generator = generator.Generator(maximum_range)
    evaluator = evaluator.Evaluator('midi_dataset.txt')
    evolver = evolver.Evolver(generator)
    population = generator.generate_population(population_size)
    grading = evaluator.grade(population)
    print(grading)
    lowest_error, best_kp = grading[0]
    generation = 0
    if lowest_error == 0:
        # First shot!
        print(best_kp)
    else:
        population = [x[1] for x in grading]
        while lowest_error > 0:
            print('Generation {}'.format(generation))
            new_population = evolver.evolve(population)
            grading = evaluator.grade(new_population)
            print(grading)
            new_lowest_error, new_best_kp = grading[0]
            if new_lowest_error <= lowest_error:
                # Let's keep just the good generations
                population = new_population
