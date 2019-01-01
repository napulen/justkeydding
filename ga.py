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
            'formatter': 'date',
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

def ga_runner(dataset, population_size, initial_key_profiles, initial_key_transitions, kp_max_range, kt_max_range, evolution_swap_threshold):
    logger.info('ga_runner() <- dataset={}, population_size={}, initial_key_profiles={}, initial_key_transitions={}, kp_max_range={}, kt_max_range={}, evolution_swap_threshold={}'.format(dataset, population_size, initial_key_profiles, initial_key_transitions, kp_max_range, kt_max_range, evolution_swap_threshold))
    # Invite all the friends to the party
    gen = generator.Generator(kp_max_range, kt_max_range)
    eva = evaluator.Evaluator(dataset)
    evo = evolver.Evolver(gen)
    # Set the initial population of key profiles
    key_profiles = initial_key_profiles
    key_profiles_length = len(key_profiles)
    if population_size < key_profiles_length:
        logger.warn('Size of initial population of key profiles greater than population size, extending population size to {}.'.format(key_profiles_length))
        population_size = key_profiles_length
    # Same for the key transitions
    key_transitions = initial_key_transitions
    key_transitions_length = len(key_transitions)
    if population_size < key_transitions_length:
        logger.warn('Size of initial population of key transitions greater than population size, extending population size to {}.'.format(key_transitions_length))
        population_size = key_transitions_length
    # Generate the remaining slots available
    remaining_key_profiles = population_size - key_profiles_length
    generated_key_profiles = gen.generate_key_profiles(remaining_key_profiles)
    key_profiles.extend(generated_key_profiles)
    remaining_key_transitions = population_size - key_transitions_length
    generated_key_transitions = gen.generate_key_transitions(remaining_key_transitions)
    key_transitions.extend(generated_key_transitions)
    logger.info('Generation 0 consists of {0} ({1} user-provided, {2} generated) key profiles and {0} ({3} user-provided, {4} generated) key transitions'.format(population_size, key_profiles_length, remaining_key_profiles, key_transitions_length, remaining_key_transitions))
    # Time to find the best kp,kt pair from the initial values
    best_key_profile = ''
    best_key_transition = ''
    lowest_error = float("inf")
    scores = []
    logger.info("Finding the best (key_profile,key_transition) from the initial populations")
    kt_grading = []
    for kt in key_transitions:
        grading = eva.grade_key_profiles(key_profiles, kt)
        lower_error, kp, _ = grading[0]
        kt_grading.append((lower_error, kt))
        if lower_error < lowest_error:
            lowest_error = lower_error
            best_key_profile = kp
            best_key_transition = kt
            scores = [x[0] for x in grading]
            key_profiles = [x[1] for x in grading]
            if lowest_error == 0:
                break
        logger.debug("({},{}) is the best pair so far".format(best_key_profile, best_key_transition))
    kt_grading = sorted(kt_grading, key=lambda score: score[0])
    key_transitions = [x[1] for x in kt_grading]
    logger.debug("sorted_key_transitions:{}".format(key_transitions))
    logger.info('From the initial populations, ({},{}) is the best (key_profile, key_transition) pair'.format(best_key_profile, best_key_transition))
    logger.debug('Scores for the initial generation: {}'.format(scores))
    kp_generation = 1
    kt_generation = 1
    bad_generation_counter = 0
    evolution_mode = 'key_profiles'
    while lowest_error > 0:
        logger.info('Key profile generation {}, Key transition generation {} - Evolving {} - Bad generations in a row: {}'.format(kp_generation, kt_generation, evolution_mode, bad_generation_counter))
        if evolution_mode == 'key_profiles':
            new_key_profiles = evo.evolve_key_profiles(key_profiles)
            new_grading = eva.grade_key_profiles(new_key_profiles, best_key_transition)
            new_scores = [x[0] for x in new_grading]
            logger.info('Scores for this generation: {}'.format(new_scores))
            new_lowest_error, new_best_kp, _ = new_grading[0]
            logger.info('Lowest error of {} from {}'.format(new_lowest_error, new_best_kp))
            if new_lowest_error < lowest_error:
                lowest_error = new_lowest_error
                best_key_profile = new_best_kp
                scores = new_scores
                key_profiles = [x[1] for x in new_grading]
                bad_generation_counter = 0
            elif new_lowest_error == lowest_error and sum(new_scores) <= sum(scores):
                lowest_error = new_lowest_error
                best_key_profile = new_best_kp
                scores = new_scores
                key_profiles = [x[1] for x in new_grading]
                bad_generation_counter = 0
            else:
                logger.warn('Performance was worst in this generation')
                bad_generation_counter += 1
                if bad_generation_counter > evolution_swap_threshold:
                    evolution_mode = 'key_transitions'
                    bad_generation_counter = 0
                    logger.warn("Changing evolution mode to {}".format(evolution_mode))
            kp_generation += 1
        elif evolution_mode == 'key_transitions':
            new_key_transitions = evo.evolve_key_transitions(key_transitions)
            new_grading = eva.grade_key_transitions(best_key_profile, new_key_transitions)
            new_scores = [x[0] for x in new_grading]
            logger.info('Scores for this generation: {}'.format(new_scores))
            new_lowest_error, _, new_best_kt = new_grading[0]
            logger.info('Lowest error of {} from {}'.format(new_lowest_error, new_best_kt))
            if new_lowest_error < lowest_error:
                lowest_error = new_lowest_error
                best_key_transition = new_best_kt
                scores = new_scores
                key_transitions = [x[2] for x in new_grading]
                bad_generation_counter = 0
            elif new_lowest_error == lowest_error and sum(new_scores) <= sum(scores):
                lowest_error = new_lowest_error
                best_key_transition = new_best_kt
                scores = new_scores
                key_transitions = [x[2] for x in new_grading]
                bad_generation_counter = 0
            else:
                logger.warn('Performance was worst in this generation')
                bad_generation_counter += 1
                if bad_generation_counter > evolution_swap_threshold:
                    evolution_mode = 'key_profiles'
                    bad_generation_counter = 0
                    logger.warn("Changing evolution mode to {}".format(evolution_mode))
            kt_generation += 1
    logger.warn('Optimization concluded, best (key_profile, key_transition) pair is ({},{})'.format(best_key_profile, best_key_transition))


if __name__ == '__main__':
    if not os.path.exists('logs'):
        os.makedirs('logs')
    logging.config.dictConfig(logging_dict)
    logger = logging.getLogger('ga_runner')
    dataset = 'midi_dataset.txt'
    population_size = 5
    kp_max_range = 100
    kt_max_range = 256
    evolution_swap_threshold = 3
    initial_key_profiles = ['sapp', 'temperley', 'krumhansl_kessler', 'aarden_essen']
    initial_key_transitions = ['exponential10']
    ga_runner(dataset, population_size, initial_key_profiles, initial_key_transitions, kp_max_range, kt_max_range, evolution_swap_threshold)
