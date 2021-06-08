import logging
import utils

#simple logger to follow on data

logging.basicConfig(filename='debugging//'+str(utils.get_current_time())+'.log',encoding='utf-8', level=logging.DEBUG)
logging.debug('Running server logger')


def add_to_log(x):
    logging.debug(x)
