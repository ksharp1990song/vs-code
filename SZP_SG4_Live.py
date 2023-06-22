import pandas as pd
import numpy as np
import random
from sklearn.model_selection import train_test_split
import xgboost as xb
import timeit

tree_method = 'hist'
m_bin = 512
xgb_obj = 'reg:squarederror'
eval_metric = "rmse"
basic_xgb_params = {'learning_rate': 0.14,
                    'max_depth': 10,
                    'min_child_weight': 1,
                    'subsample': 0.8,
                    'colsample_bytree': 1,
                    'n_estimators': 150,
                    'gamma': 0,
                    'num_parallel_tree': 10,
                    'reg_lambda': 1,
                    'reg_alpha': 0}

learning_rate = basic_xgb_params['learning_rate']
max_depth = basic_xgb_params['max_depth']
min_child_weight = basic_xgb_params['min_child_weight']
subsample = basic_xgb_params['subsample']
colsample_bytree = basic_xgb_params['colsample_bytree']
n_estimators = basic_xgb_params['n_estimators']
reg_lambda = basic_xgb_params['reg_lambda']
reg_alpha = basic_xgb_params['reg_alpha']
gamma = basic_xgb_params['gamma']
num_parallel_tree = basic_xgb_params['num_parallel_tree']

ml_model_NG = xb.XGBClassifier(objective=xgb_obj, tree_method=tree_method, verbosity=1, eval_metric=eval_metric,
                               num_parallel_tree=num_parallel_tree, n_estimators=n_estimators,
                               learning_rate=learning_rate, max_bin=m_bin, max_depth=max_depth,
                               min_child_weight=min_child_weight, gamma=gamma, subsample=subsample,
                               colsample_bytree=colsample_bytree, reg_lambda=reg_lambda, reg_alpha=reg_alpha)

ml_model_NR = xb.XGBClassifier(objective=xgb_obj, tree_method=tree_method, verbosity=1, eval_metric=eval_metric,
                               num_parallel_tree=num_parallel_tree, n_estimators=n_estimators,
                               learning_rate=learning_rate, max_bin=m_bin, max_depth=max_depth,
                               min_child_weight=min_child_weight, gamma=gamma, subsample=subsample,
                               colsample_bytree=colsample_bytree, reg_lambda=reg_lambda, reg_alpha=reg_alpha)


def loadSZPModel():
    ml_model_NG.load_model('trained_Model_SZPNG4.txt')
    ml_model_NR.load_model('trained_Model_SZPNR4.txt')


def predictNG(tx, sb1, sb2, sb3, sb4, sb5, sb6, gs1, gs2, gs3, gs4, gs5, gs6, rs1, rs2, rs3, rs4, rs5, rs6):

    data = [[tx, sb1, sb2, sb3, sb4, sb5, sb6, gs1, gs2, gs3, gs4, gs5, gs6, rs1, rs2, rs3, rs4, rs5, rs6]]
    df = pd.DataFrame(data, columns=["tx", "SB1", "SB2", "SB3", "SB4", "SB5", "SB6",
                                            "GS1", "GS2", "GS3", "GS4", "GS5", "GS6",
                                            "RS1", "RS2", "RS3", "RS4", "RS5", "RS6"])
    ng_pred = ml_model_NG.predict(df)

    return int(ng_pred[0])


def predictNR(tx, sb1, sb2, sb3, sb4, sb5, sb6, gs1, gs2, gs3, gs4, gs5, gs6, rs1, rs2, rs3, rs4, rs5, rs6):
    data = [[tx, sb1, sb2, sb3, sb4, sb5, sb6, gs1, gs2, gs3, gs4, gs5, gs6, rs1, rs2, rs3, rs4, rs5, rs6]]
    df = pd.DataFrame(data, columns=["tx", "SB1", "SB2", "SB3", "SB4", "SB5", "SB6",
                                     "GS1", "GS2", "GS3", "GS4", "GS5", "GS6",
                                     "RS1", "RS2", "RS3", "RS4", "RS5", "RS6"])
    nr_pred = ml_model_NR.predict(df)

    return int(nr_pred[0])


