import string
import random
import subprocess

def generate_key_profiles(maximum_range):
    ''' Generate a set of random key profiles '''
    major = generate_major_key_profile(maximum_range)
    minor = generate_minor_key_profile(maximum_range)
    return major + minor

def generate_major_key_profile(maximum_range):
    key_profile = [
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ]
    tonic_dominant = [0, 7]
    diatonic = [2, 4, 5, 9, 11]
    non_diatonic = [1, 3, 6, 8, 10]
    groups = [
        (tonic_dominant, 0),
        (diatonic, 1),
        (non_diatonic, 2)
    ]
    for group, hierarchy in groups:
        for idx in group:
            r = random.randrange(int(maximum_range / 2**hierarchy))
            key_profile[idx] = r
    s = sum(key_profile)
    return [x/s for x in key_profile]

def generate_minor_key_profile(maximum_range):
    key_profile = [
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    ]
    tonic_dominant = [0, 7]
    harmonic = [2, 3, 5, 8, 11]
    natural_melodic = [4, 9, 10]
    non_diatonic = [1, 6]
    groups = [
        (tonic_dominant, 0),
        (harmonic, 1),
        (natural_melodic, 2),
        (non_diatonic, 3)
    ]
    for group, hierarchy in groups:
        for idx in group:
            r = random.randrange(int(maximum_range / 2**hierarchy))
            key_profile[idx] = r
    s = sum(key_profile)
    return [x/s for x in key_profile]

def generate_population(count, maximum_range):
    ''' Generate <count> key_profiles for a population '''
    return [generate_key_profiles(maximum_range) for x in range(count)]

def grade(population):
    ''' Grade a whole population '''
    # summed = reduce(lambda x,y:x+y, (fitness(x, target) for x in pop), 0)
    # return summed / float(len(pop))
    grading = [(x, evaluate(x)) for x in population]
    return grading

def evaluate(key_profile):
    ''' Evaluate a key profile '''
    kp_string = ''
    for p in key_profile:
        kp_string = kp_string + '{:.12f} '.format(p)
    kp_string = kp_string[:-1]
    #print('"{}"'.format(kp_string))
    justkeydding = subprocess.Popen(
        ('bin/justkeydding',
        '-e',
        '-K',
        '{}'.format(kp_string),
        'Test_the_keys.mid'),
        stdout=subprocess.PIPE)
    output, _ = justkeydding.communicate()
    try:
        score = float(output)
    except ValueError:
        score = 0.0
    error = 1 - score
    return error

# def cmpchars(c1, c2):
#     ''' Compare the ascii values of two characters '''
#     return abs(ord(c1)-ord(c2))

# def fitness(hypotheses, target):
#     ''' Outputs the fitness of a particular hypotheses '''
#     z = 0
#     for idx in range(len(target)):
#         z = z + cmpchars(hypotheses[idx],target[idx])
#     return z

def evolve(pop, target, retain=0.5, random_select=0.02, mutate=0.1):
    ''' Do the evolution! '''
    graded = [ (fitness(x, target), x) for x in pop]
    graded = [ x[1] for x in sorted(graded)]
    retain_length = int(len(graded)*retain)
    parents = graded[:retain_length]

    # add weak hypotheses with <random_select> chance
    for hypotheses in graded[retain_length:]:
        if random_select > random.random():
            parents.append(hypotheses)

    # mutate some hypotheses
    for idx,hypotheses in enumerate(parents):
        if mutate > random.random():
            pos_to_mutate = random.randint(0, len(hypotheses)-1)
            hypotheses = list(hypotheses)
            hypotheses[pos_to_mutate] = random.choice(alphabet)
            hypotheses = ''.join(hypotheses)
            parents[idx] = hypotheses

    # crossover parents to fill the rest of the population
    parents_length = len(parents)
    desired_length = len(pop) - parents_length
    children = []
    while len(children) < desired_length:
        male = random.randint(0, parents_length-1)
        female = random.randint(0, parents_length-1)
        if male != female:
            male = parents[male]
            female = parents[female]
            half = len(male) / 2
            child = male[:half] + female[half:]
            children.append(child)
    parents.extend(children)
    return parents

def main():
    # What we are trying to achieve
    target = 'unpasswordbienchingon'
    # Generate the first population
    pop = population(10, len(target))
    g = grade(pop, target)
    # Evolve until you reach the target ;)!
    while g > 0:
        pop = evolve(pop, target)
        g = grade(pop, target)
        print(pop[0])

# The program starts running here!
# main() is a function, and this if statement will call to that function
if __name__ == '__main__':
    population_size = 10
    maximum_range = 100
    population = generate_population(population_size, maximum_range)
    grading = grade(population)
    for entry in grading:
        print(entry)

