import seaborn as sns
import matplotlib.pyplot as plt
import optimizer.key_profiles as kp
import pandas as pd

if __name__ == '__main__':
    six_profiles = {
        'aarden_essen': 'Aarden-Essen',
        'albrecht_shanahan2': 'Albrecht-Shanahan',
        'bellman_budge': 'Bellman-Budge',

        'krumhansl_kessler': 'Krumhansl-Kessler',
        'sapp': 'Sapp',
        'temperley': 'Temperley'
    }
    kps = { six_profiles[k]: v for k, v in kp._kp.items() if k in six_profiles }
    df = pd.DataFrame(kps)
    df['Pitch-Class'] = list(range(12)) * 2
    df['Mode'] = ['Major'] * 12 + ['Minor'] * 12
    df = df.melt(id_vars=['Pitch-Class', 'Mode'], var_name='Key Profile', value_name='Probability')
    df_major = df.loc[df['Mode'] == 'Major']
    df_minor = df.loc[df['Mode'] == 'Minor']

    sns.catplot(x='Pitch-Class', y='Probability', hue='Key Profile', palette='colorblind', row='Mode', height=2, aspect=2.3, legend_out=True, data=df, kind='swarm')
    # sns.swarmplot(x='Pitch-Class', y='Probability', hue='Key Profile', palette='colorblind', data=df)
    # sns.swarmplot(x='Pitch-Class', y='Probability', hue='Key Profile', palette='colorblind', data=df_minor, marker=('X'))
    plt.show()

