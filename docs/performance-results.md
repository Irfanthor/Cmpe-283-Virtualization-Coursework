# Performance Results

Use `scripts/benchmark.sh` to capture baseline scheduler performance and execution behavior.

Suggested reporting table:

| Scheduler | Runtime (s) | VM A Retired Instructions | VM B Retired Instructions | Notes |
|-----------|-------------|---------------------------|---------------------------|-------|
| round_robin | TBD | TBD | TBD | fairness baseline |
| priority | TBD | TBD | TBD | high-priority VM preference |
| mlfq | TBD | TBD | TBD | interactive bias, adaptive behavior |

Migration metrics are exported as duration in milliseconds from `MetricsTracer::LastMigrationDuration()`.
