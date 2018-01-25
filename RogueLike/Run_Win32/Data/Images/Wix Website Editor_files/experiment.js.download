define(['lodash'], function (_) {
    'use strict';

    var rawRunningExperiments = getRunningExperiments();
    var transformedRunningExperiments = transformRunningExperiments(rawRunningExperiments);

    function isOpen(name, context) {
        return getValue(name, context) === 'new';
    }

    function getValue(name, context) {
        if (rawRunningExperiments !== getRunningExperiments(context)) {
            rawRunningExperiments = getRunningExperiments(context);
            transformedRunningExperiments = transformRunningExperiments(getRunningExperiments(context));
        }
        return transformedRunningExperiments[name.toLowerCase()];
    }

    function isMultiValueExperimentOpen(name, context) {
        var value = getValue(name, context);
        return value && value !== 'old';
    }

    function getRunningExperiments(context) {
        var expFromcontext = _.get(context, 'rendererModel.runningExperiments');

        if (expFromcontext) {
            return expFromcontext;
        }

        if (typeof window === 'undefined') {
            return {};
        }

        return (window.rendererModel || window.editorModel || {}).runningExperiments;
    }

    function transformRunningExperiments(runningExperiments) {
        return _.mapKeys(runningExperiments, function (value, key) {
            return key.toLowerCase();
        });
    }

    return {
        isOpen: isOpen,
        getValue: getValue,
        isMultiValueExperimentOpen: isMultiValueExperimentOpen
    };
});
