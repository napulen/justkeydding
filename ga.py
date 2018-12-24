from optimizer import generator, evaluator, evolver
import os
import logging
import logging.handlers
import logging.config

logging_dict = {
    'version': 1,
    'disable_existing_loggers': False,
    'formatters': {
        'date': {
            'format': '%(asctime)s - [%(name)-9s] %(levelname)-5s:  %(message)s',
        },
        'no_date': {
            'format': '[%(name)-9s] %(levelname)-5s:  %(message)s',
        },
    },
    'handlers': {
        'file_handler': {
            'class': 'logging.handlers.TimedRotatingFileHandler',
            'filename': 'logs/optimizer.log',
            'when': 'H',
            'interval': 1,
            'backupCount': 360,
            'formatter': 'no_date',
            'encoding': 'utf8'
        },
        'console': {
            'class': 'logging.StreamHandler',
            'level': 'DEBUG',
            'formatter': 'no_date'
        }
    },
    'loggers': {
        '': {
            'handlers': ['file_handler', 'console'],
            'level': 'DEBUG',
            'propagate': True
        }
    }
}

def ga_runner(dataset, maximum_range, population_size, initial_population):
    logger.info('ga_runner() <- population_size={}, maximum_range={}, dataset={}'.format(population_size, maximum_range, dataset))
    gen = generator.Generator(maximum_range)
    eva = evaluator.Evaluator(dataset)
    evo = evolver.Evolver(gen)
    population = initial_population
    initial_population_length = len(initial_population)
    if population_size < initial_population_length:
        logger.warn('Size of initial population greater than population size, extending population size to {}.'.format(initial_population_length))
        population_size = initial_population_length
    generated_population_length = population_size - len(initial_population)
    generated_population = gen.generate_population(generated_population_length)
    population.extend(generated_population)
    logger.info('Generation 0 consists of {} user-provided key profiles and {} generated key profiles'.format(initial_population_length, generated_population_length))
    grading = eva.grade(population)
    scores = [x[0] for x in grading]
    logger.debug('Scores for this generation: {}'.format(scores))
    lowest_error, best_kp = grading[0]
    logger.info('Lowest error of {} from {}'.format(lowest_error, best_kp))
    generation = 1
    population = [x[1] for x in grading]
    while lowest_error > 0:
        logger.info('Generation {}'.format(generation))
        new_population = evo.evolve(population)
        new_grading = eva.grade(new_population)
        new_scores = [x[0] for x in new_grading]
        logger.info('Scores for this generation: {}'.format(new_scores))
        new_lowest_error, new_best_kp = new_grading[0]
        logger.info('Lowest error of {} from {}'.format(new_lowest_error, new_best_kp))
        if new_lowest_error <= lowest_error:
            lowest_error = new_lowest_error
            population = [x[1] for x in new_grading]
        else:
            logger.warn('Performance was worst in this generation, using the previous one')
        generation += 1
    logger.warn('Optimization concluded, best key profile is {}'.format(best_kp))


if __name__ == '__main__':
    if not os.path.exists('logs'):
        os.makedirs('logs')
    logging.config.dictConfig(logging_dict)
    logger = logging.getLogger('ga_runner')
    population_size = 10
    maximum_range = 100
    dataset = 'midi_dataset.txt'
    initial_population = []
    ga_runner(dataset, maximum_range, population_size, initial_population)
