import seaborn as sns
import matplotlib.pyplot as plt
import optimizer.key_profiles as kp
import pandas as pd

if __name__ == '__main__':
    six_profiles = [
        'aarden_essen', 'albrecht_shanahan2', 
        'bellman_budge', 'krumhansl_kessler', 
        'sapp', 'temperley'
    ]
    kps = { k: v for k, v in kp._kp.items() if k in six_profiles }
    df = pd.DataFrame(kps)
    df['pitchclass'] = list(range(12)) * 2
    df['mode'] = ['major'] * 12 + ['minor'] * 12
    df = df.melt(id_vars=['pitchclass', 'mode'], var_name='keyprofile', value_name='probability')
    df_major = df.loc[df['mode'] == 'major']
    df_minor = df.loc[df['mode'] == 'minor']
    
    ax = sns.catplot(x='pitchclass', y='probability', height=4.0, aspect=2.0, legend_out=False, hue='mode', data=df, kind='swarm')
    plt.show()

